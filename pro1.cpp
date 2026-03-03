#include <cstdio>
#include <cstdlib>
#include <time.h>
typedef struct Item {
  int v;
  struct Item *next; } item;
typedef struct stack_data {
  item *top;
  int min;
  unsigned int size; } data;

static struct Item *min_list_tail;

static void free_min_list(void) {
  if(min_list_tail == NULL) return;
  item *p = min_list_tail->next;
  while(min_list_tail != NULL) {
    free(min_list_tail);
    min_list_tail = p;
    if(p == NULL) break;
    p = p->next; }
  min_list_tail = NULL; }

static void update_min_list(data *stack,bool append_or_delete,int var) {
  if(append_or_delete == 1) {
    item *p = (item*)malloc(sizeof(item));
    p->v = var;
    if(min_list_tail == NULL) {
      p->next = NULL; min_list_tail = p; }
    else {
      p->next = min_list_tail; }
    min_list_tail = p;
    stack->min = min_list_tail->v; }
  else {
    item *d = min_list_tail;
    min_list_tail = min_list_tail->next;
    stack->min = min_list_tail->v;
    free(d); } }

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
  stack->min = NULL; }

void push(data *stack,int var) {
  item *p = (item*)malloc(sizeof(item));
  p->v = var;
  if(stack->top == NULL) {
    stack->top = p; stack->min = p->v; stack->top->next = NULL;
    update_min_list(stack,1,p->v); }
  else {
    p->next = stack->top;
    stack->top = p;
    if((p->v) < (stack->min)) {
      update_min_list(stack,1,p->v); } } }

int pop(data *stack) {
  item *p = stack->top;
  stack->top = stack->top->next;
  if(p->v == stack->min) {
    update_min_list(stack,0,0); }
  free(p);
  return stack->top->v; }

void print_stack(item *top) {
  item *p = top;
  while(p != NULL) {
    printf("%d ",p->v); p = p->next; } }

int stack_find_min(void) {
  return min_list_tail->v; }

int main(void) {
  data stack; // stack variable
  min_list_tail = NULL; // init min numbers list
  srand((unsigned)time(NULL)); // init random numbers seed
  init_stack(&stack); int a = 0; // init stack and temp variable
  for(int i = 0; i<11; ++i) {
    a = rand() % 51 + 13; // generate a random number sequence of 13-63
    push(&stack,a); } // push numbers
  /*
    you can do things here
  */
  free_stack(&stack); // free stack memory
  return 0; }
