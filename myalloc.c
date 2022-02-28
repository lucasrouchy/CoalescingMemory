#include "myalloc.h"
struct block *head = NULL;

void split_space(struct block* curr, int requested_size){
  int available = curr->size;
  int requested_padded_size = PADDED_SIZE(requested_size);
  int padded_struct_block_size = PADDED_SIZE(sizeof(struct block));
  int min_size = requested_padded_size + padded_struct_block_size;
  if (available < min_size + 16) return;

  struct block *split_node = PTR_OFFSET(curr, min_size);
  split_node->size = available - min_size;
  split_node->next = curr->next;
  curr->size = requested_padded_size;
  curr->next = split_node;

}

void *myalloc(int num){
    if (head == NULL) {
      head = sbrk(1024);
      head->next = NULL;
      head->size = 1024 - PADDED_SIZE(sizeof(struct block));
      head->in_use = 0;
    }
    struct block *current = head;

    int padded_num = PADDED_SIZE(num);

    while (current != NULL){
      if (current->in_use == 0 && current->size >= padded_num){
        split_space(current, num);
        current->in_use = 1;
        int padded_struct_block_size = PADDED_SIZE(sizeof(struct block));
        return PTR_OFFSET(current, padded_struct_block_size);

      }
      current = current->next;
    }
    return NULL;

}

void print_data(void)
{
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}


void myfree(void *p){
  struct block *freenode = p - PADDED_SIZE(sizeof(struct block));
  freenode->in_use = 0;
  int padded_amount = PADDED_SIZE(sizeof(struct block));
  struct block *cur = head;
  while (cur->next != NULL){
    if ((cur->in_use ==  0)  && (cur->next->in_use ==  0)){
        cur->size += padded_amount + (cur->next)->size;
        cur->next = cur->next->next;
        
    }
    else{
      cur = cur->next;
    }
  }

}
int main(void){
  
  void *p, *q, *r, *s;

  p = myalloc(10); print_data();
  q = myalloc(20); print_data();
  r = myalloc(30); print_data();
  s = myalloc(40); print_data();

  myfree(q); print_data();
  myfree(p); print_data();
  myfree(s); print_data();
  myfree(r); print_data();
    return 0;

}
