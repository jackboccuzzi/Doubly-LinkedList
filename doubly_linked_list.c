/* doubly_linked_list.c
 *
 * Implement a sorted linked list of strings with operations Insert 
 * in alphabetical order, Print, Member, Delete, Free_list.
 * The list nodes are doubly linked.
 * 
 * Input:    Single character lower case letters to indicate operations, 
 *           possibly followed by value needed by operation -- e.g. 'i'
 *           followed by "hello" to insert the string "hello" -- no 
 *           double or single quotes.
 *
 * Output:   Results of operations.
 *
 * Compile:  gcc -g -Wall -o doubly_linked_list doubly_linked_list.c
 *           
 * Run:      ./doubly_linked_list
 *
 */

/* You may not add any new header file */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Max 99 chars + '\0' */
#define STRING_MAX 100

struct list_node_s {
   char*  data;
   struct list_node_s* prev_p;
   struct list_node_s* next_p;
};

/* Pointers to the head and tail of the list */
struct list_s {
   struct list_node_s* h_p;
   struct list_node_s* t_p;
};

void Insert(struct list_s* list_p, char string[]);
void Print(struct list_s* list_p);
int  Member(struct list_s* list_p, char string[]);
void Delete(struct list_s* list_p, char string[]);
void Free_list(struct list_s* list_p);
char Get_command(void);
void Get_string(char string[]);
void Free_node(struct list_node_s* node_p);
struct list_node_s* Allocate_node(int size);
void Print_node(char title[], struct list_node_s* node_p);


/*-----------------------------------------------------------------*/
int main(void) {
   char          command;
   char          string[STRING_MAX];
   struct list_s list;  

   list.h_p = list.t_p = NULL;
      /* start with empty list */

   command = Get_command();
   while (command != 'q' && command != 'Q') {
      switch (command) {
         case 'i': 
         case 'I': 
            Get_string(string);
            Insert(&list, string);
            break;
         case 'p':
         case 'P':
            Print(&list);
            break;
         case 'm': 
         case 'M':
            Get_string(string);
            if (Member(&list, string))
               printf("%s is in the list\n", string);
            else
               printf("%s is not in the list\n", string);
            break;
         case 'd':
         case 'D':
            Get_string(string);
            Delete(&list, string);
            break;
         case 'f':
         case 'F':
            Free_list(&list);
            break;
         default:
            printf("There is no %c command\n", command);
            printf("Please try again\n");
      }
      command = Get_command();
   }
   Free_list(&list);

   return 0;
}  /* main */


/*-----------------------------------------------------------------*/
/* Function:   Allocate_node
 * Purpose:    Allocate storage for a list node
 * Input arg:  size = number of chars needed in data member (including
 *                storage for the terminating null)
 * Return val: Pointer to the new node
 */
struct list_node_s* Allocate_node(int size) {

   // allocate memory for the node
   struct list_node_s* allocated_node = (struct list_node_s*) malloc(sizeof(struct list_node_s));

   if (allocated_node == NULL) {
      printf("Memory not allocated for the node.");
      return NULL;
   }

   // allocate memoery for the string contained in the node
   allocated_node->data = (char*) malloc(sizeof(char) * size);

   if (allocated_node->data == NULL) {
      printf("Memory not allocated for the Node's data.");
      return NULL;
   }

   // initialize the next and prev pointers for the string.
   allocated_node->next_p = NULL;
   allocated_node->prev_p = NULL;

   return allocated_node;
}  /* Allocate_node */


/*-----------------------------------------------------------------*/
/* Function:   Insert
 * Purpose:    Insert new node in correct alphabetical location in list
 * Input arg:  string = new string to be added to list
 * In/out arg: list_p = pointer to struct storing head and tail ptrs
 * Note:       If the string is already in the list, print a message
 *                and return, leaving list unchanged
 */
void Insert(struct list_s* list_p, char string[]) { 
   // create the new node to insert
   struct list_node_s* newNode = Allocate_node((int) strlen(string) + 1);
   // check if memory was allocated
   if (newNode == NULL) {
      printf("Memory not allocated for node.");
      return;
   }
   // add the new node's data
   strcpy(newNode->data, string);

   // if the list is empty, set head and tail pointers to the only node
   if (list_p->h_p == NULL) {
      list_p->h_p = newNode;
      list_p->t_p = newNode;
      return;
   }

   struct list_node_s* currentNode = list_p->h_p;
   // iterate through the list until the point you have to enter...
   while (currentNode->next_p != NULL && strcmp(newNode->data, currentNode->data) > 0) {
      currentNode = currentNode->next_p;
   }

   // if the string exists at the current node...
   if (strcmp(newNode->data, currentNode->data) == 0) {
      printf("This string already exists in the list.\n");
      return;
   }

   // if inserting at the head...
   else if (list_p->h_p == currentNode && strcmp(newNode->data, currentNode->data) < 0) {
      // set the pointers for the new node
      currentNode->prev_p = newNode;
      newNode->next_p = currentNode;
      list_p->h_p = newNode;
   }

   // if inserting at the tail...
   else if (list_p->t_p == currentNode) {
      if (strcmp(newNode->data, currentNode->data) < 0) {
         // new node's next becomes current node
         newNode->next_p = currentNode;
         // new node's prev becomes current's prev
         newNode->prev_p = currentNode->prev_p;
         // the current node's prev's next becomes the newNode
         (currentNode->prev_p)->next_p = newNode;
         // the current node's prev now becomes the newNode
         currentNode->prev_p = newNode;
      }
      else {
         // set the pointers for the new node
         currentNode->next_p = newNode;
         newNode->prev_p = currentNode;
         list_p->t_p = newNode;
      }
   }

   // otherwise just add it into the list...
   else {
      newNode->next_p = currentNode->next_p;
      newNode->prev_p = currentNode;
      // the node after the current node... its prev is the new node
      (currentNode->next_p)->prev_p = newNode;
      currentNode->next_p = newNode;
   }

}  /* Insert */

/*-----------------------------------------------------------------*/
/* Function:   Print
 * Purpose:    Print the contents of the nodes in the list
 * Input arg:  list_p = pointers to first and last nodes in list
 *
 * Hint: The implementation of this function (below) shows how
 * to traverse a linked list.
 */
void Print(struct list_s* list_p) {
   struct list_node_s* curr_p = list_p->h_p;

   printf("list = ");

   while (curr_p != NULL) {
      printf("%s ", curr_p->data);
      curr_p = curr_p->next_p;
   }
   printf("\n");
}  /* Print */


/*-----------------------------------------------------------------*/
/* Function:   Member
 * Purpose:    Search list for string
 * Input args: string = string to search for
 *             list_p = pointers to first and last nodes in list
 * Return val: 1, if string is in the list, 0 otherwise
 */
int Member(struct list_s* list_p, char string[]) {
   // start at the head node...
   struct list_node_s* currentNode = list_p->h_p;
   // iterate through the linked list...
   while (currentNode != NULL && strcmp(string, currentNode->data) >= 0) {
      // if we find the desired string...
      if (strcmp(currentNode->data, string) == 0) {
         return 1;
      }
      currentNode = currentNode->next_p;
   }
   // if we dont find it...
   return 0;
}  /* Member */


/*-----------------------------------------------------------------*/
/* Function:   Free_node
 * Purpose:    Free storage used by a node of the list
 * In/out arg: node_p = pointer to node to be freed
 */
void Free_node(struct list_node_s* node_p) {
   free(node_p->data);
   free(node_p);
}  /* Free_node */


/*-----------------------------------------------------------------*/
/* Function:   Delete
 * Purpose:    Delete node containing string.
 * Input arg:  string = string to be deleted
 * In/out arg  list_p = pointers to head and tail of list
 * Note:       If the string is in the list, it will be unique.  So
 *             at most one node will be deleted.  If the string isn't
 *             in the list, the function just prints a message and
 *             returns, leaving the list unchanged.
 */
void Delete(struct list_s* list_p, char string[]) {
   struct list_node_s* currentNode = list_p->h_p;

   // if the list is empty...
   if (currentNode == NULL) {
      printf("The string is not in the list.\n");
      return;
   }

   // iterate through the list until the point you have to delete...
   while (currentNode != NULL && strcmp(string, currentNode->data) > 0) {
      currentNode = currentNode->next_p;
   }
   
   // if the string is not in the list after iterating...
   if (currentNode == NULL) {
      printf("The string is not in the list.\n");
      return;
   }

   // after iterating to the correct spot, if the string does not exist...
   if (strcmp(string, currentNode->data) != 0) {
      printf("The string is not in the list.\n");
      return;
   }

   // if we delete from the head...
   if (list_p->h_p == currentNode) {
      // check if next is null..
      if (currentNode->next_p == NULL) {
            list_p->h_p = NULL;
            list_p->t_p = NULL;
      }
      // other wise just set the head to the next node...
      else {
         list_p->h_p = currentNode->next_p;
         (list_p->h_p)->prev_p = NULL;
      }
   }

   // if we delete from the tail
   else if (list_p->t_p == currentNode) {
      // check if prev is null...
      if (currentNode->prev_p == NULL) {
         list_p->t_p = NULL;
         list_p->h_p = NULL;
      }
      else {
         // set the tail pointer to the current tail's prev node
         list_p->t_p = currentNode->prev_p;
         // set the new tail's next to NULL
         (list_p->t_p)->next_p = NULL;
         // set the deleted node's prev to NULL;
         currentNode->prev_p = NULL;
      }
   }

   // otherwise conduct a normal delete...
   else {
      // the current node's prev points to current node's next...
      (currentNode->prev_p)->next_p = currentNode->next_p;
      // the next node's prev, now points to current node's prev...
      (currentNode->next_p)->prev_p = currentNode->prev_p;
      // set the currentNode's pointers to NULL to finish deletion...
      currentNode->next_p = NULL;
      currentNode->prev_p = NULL;
   }

   Free_node(currentNode);
}  /* Delete */

/*-----------------------------------------------------------------*/
/* Function:   Free_list
 * Purpose:    Free storage used by list
 * In/out arg: list_p = pointers to head and tail of list
 */
void Free_list(struct list_s* list_p) {
   while(list_p->h_p != NULL) {
      Delete(list_p, list_p->h_p->data);
   }
}  /* Free_list */


/*-----------------------------------------------------------------*/
/* Function:   Get_command
 * Purpose:    Find and return the next non-whitespace character in
 *             the input stream
 * Return val: The next non-whitespace character in stdin
 */
char Get_command(void) {
   char c;

   printf("Please enter a command (i, d, m, p, f, q):  ");
   /* Put the space before the %c so scanf will skip white space */
   scanf(" %c", &c);
   return c;
}  /* Get_command */

/*-----------------------------------------------------------------*/
/* Function:   Get_string
 * Purpose:    Read the next string in stdin (delimited by whitespace)
 * Out arg:    string = next string in stdin
 */
void Get_string(char string[]) {

   printf("Please enter a string:  ");
   scanf("%s", string);
}  /* Get_string */


/*-----------------------------------------------------------------*/
/* Function:  Print_node
 * Purpose:   Print the data member in a node or NULL if the
 *            pointer is NULL
 * In args:   title:  name of the node
 *            node_p:  pointer to node
 */
void Print_node(char title[], struct list_node_s* node_p) {
   printf("%s = ", title);
   if (node_p != NULL)
      printf("%s\n", node_p->data);
   else
      printf("NULL\n");
}  /* Print_node */

