#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define DELETE 0
#define UPDATE 1
typedef struct Item {
  int v;
  struct Item *next; } item;

typedef struct stack_data {
  int min;
  unsigned int size;
  item *top; } data;

typedef struct Min_Item {
  int v;
  item *address;
  struct Min_Item *next; } min_item;

static struct Min_Item *min_list_tail;
static unsigned int min_list_size = 0;
static bool is_min_and_push = false;

void print_min_list(void) {
  min_item *p = min_list_tail;
  while(p != NULL) {
    printf("%d\n",p->v); p = p->next; } }

static void free_min_list(void) {
  if(min_list_tail == NULL) return;
  min_item *p = min_list_tail->next;
  while(min_list_tail != NULL) {
    free(min_list_tail);
    min_list_tail = p;
    if(p == NULL) break;
    p = p->next; }
  min_list_tail = NULL; }

static void update_min_list(data *stack,unsigned int things,item *min) {
  min_item *p = (min_item*)malloc(sizeof(min_item));
  min_item *d = min_list_tail;
  switch(things) {
  case DELETE:
    min_list_tail = min_list_tail->next;
    stack->min = min_list_tail->v; --min_list_size;
    free(d);
    break;
  case UPDATE:
    p->v = min->v;
    if(min_list_tail == NULL) {
      p->next = NULL; min_list_tail = p;
      min_list_tail->address = NULL;
      ++min_list_size; }
    else {
      p->next = min_list_tail;
      min_list_tail = p;
      ++min_list_size; }
    stack->min = min_list_tail->v;
    break; } }

void delete_min(data *stack) {
  struct Item *p = min_list_tail->address;
  item *s = p->next;
  if(p->next->next == NULL) {
    p->next = NULL; }
  else {
    p->next = p->next->next; }
  free(s);
  update_min_list(stack,DELETE,NULL); }

void free_stack(data *stack) {
  item *p = stack->top->next;
  while(stack->top != NULL) {
    free(stack->top);
    stack->top = p;
    if(p == NULL) break;
    p = p->next; }
  stack->size = 0;
  free_min_list(); }

void init_stack(data *stack) {
  stack->size = 0;
  stack->top = NULL;
  stack->min = INT_MIN;
  min_list_tail = NULL; }

void push(data *stack,int var) {
  item *p = (item*)malloc(sizeof(item));
  p->v = var;
  if(stack->top == NULL) {
    stack->top = p; stack->min = p->v; stack->top->next = NULL;
    is_min_and_push = true;
    update_min_list(stack,UPDATE,p); }
  else {
    if(is_min_and_push == true) {
      min_list_tail->address = p;
      is_min_and_push = false; }
    p->next = stack->top;
    stack->top = p;
    if((p->v) < (stack->min)) {
      is_min_and_push = true;
      update_min_list(stack,UPDATE,p); } } }

int pop(data *stack) {
  item *p = stack->top;
  stack->top = stack->top->next;
  if(p->v == stack->min) {
    update_min_list(stack,DELETE,NULL); }
  free(p);
  return stack->top->v; }

void print_stack(item *top) {
  item *p = top;
  while(p != NULL) {
    printf("%d\n",p->v); p = p->next; } }

int stack_find_min(void) {
  return min_list_tail->v; }

int main(void) {
  data stack; // stack variable
  srand((unsigned)time(NULL)); // init random numbers seed
  init_stack(&stack); int a = 0; // init stack and temp variable
  for(int i = 0; i<11; ++i) {
    a = rand() % 51 + 13; // generate a random number sequence of 13-63
    push(&stack,a); } // push numbers
  print_stack(stack.top);
  printf("\n");
  print_min_list();
  /*delete_min(&stack);
  printf("\n");
  print_stack(stack.top);
  printf("\n");
  print_min_list();*/
  free_stack(&stack); // free stack memory
  return 0; }
