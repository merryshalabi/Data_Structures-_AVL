#include "avl.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum Position { LEFT, RIGHT } Position;

int max(int x, int y)
{
    return (x < y) ? y : x;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}
/****
  Student1 name: Merry Shalabi

  Student1 ID: 324007202
****/


/* returns the size of a node, if it's NULL then it returns 0.*/
int getSize(AVLNodePtr tnode)
{
    if (!tnode)
        return 0;
    return tnode->size;
}
/* returns the height of a node, if it's NULL then it returns -1.*/
int getHeight(AVLNodePtr tnode)
{
    if (!tnode)
        return -1;
    return tnode->height;
}
/* This function updating the size of the given node.*/
int update_size(AVLNodePtr tnode)
{
    return (getSize(tnode->child[RIGHT])+getSize(tnode->child[LEFT]) + 1);
}
/* This function updating the height of the given node.*/
int update_height(AVLNodePtr tnode)
{
    return (max(getHeight(tnode->child[LEFT]), getHeight(tnode->child[RIGHT])) + 1);
}
/*This function returns a pointer to the min value on the tree*/
AVLNodePtr avl_min(AVLNodePtr tnode)
{
    if (tnode->child[LEFT]==NULL)
    {
        return tnode;
    }
    return avl_min(tnode->child[LEFT]);
}
/*This function returns a pointer to the max value on the tree*/
AVLNodePtr avl_max(AVLNodePtr tnode)
{
    if (tnode->child[RIGHT]==NULL)
    {
        return tnode;
    }
    return avl_max(tnode->child[RIGHT]);
}
/*left rotation*/
AVLNodePtr left_rotate(AVLNodePtr tnode)
{
    AVLNodePtr temp = tnode->child[RIGHT]; /*swap pointer*/
    tnode->child[RIGHT] = temp->child[LEFT];
    temp->child[LEFT] = tnode;

    tnode->height = update_height(tnode);/* update size and height*/
    temp->height = update_height(temp);
    tnode->size = update_size(tnode);
    temp->size = update_size(temp);

    return temp;
}
/*right rotation*/
AVLNodePtr right_rotate(AVLNodePtr tnode)
{
    AVLNodePtr temp = tnode->child[LEFT];/*swap pointer*/
    tnode->child[LEFT] = temp->child[RIGHT];
    temp->child[RIGHT] = tnode;


    tnode->height = update_height(tnode);/* update size and height*/
    temp->height = update_height(temp);
    tnode->size = update_size(tnode);
    temp->size = update_size(temp);

    return temp;
}
/*this function returns a balanced tree*/
AVLNodePtr balance(AVLNodePtr tnode)
{
    if (abs(getHeight(tnode->child[LEFT]) - getHeight(tnode->child[RIGHT])) < 2)
        return tnode; /* if the tree is balanced return*/
    else {
        if (getHeight(tnode->child[LEFT]) > getHeight(tnode->child[RIGHT]))
        {
            if (getHeight(tnode->child[LEFT]->child[RIGHT]) > getHeight(tnode->child[LEFT]->child[LEFT]))
               tnode->child[LEFT]= left_rotate(tnode->child[LEFT]); /* the tree is left heavy and the left subtree is right heavy as left rotation to the left subtree then right rotation to the tree*/
            return right_rotate(tnode);/* the tree and the left subtree are left heavy then right rotation to the tree*/
        }

        if(getHeight(tnode->child[RIGHT]) > getHeight(tnode->child[LEFT]))
        {
            if  (getHeight(tnode->child[RIGHT]->child[LEFT]) > getHeight(tnode->child[RIGHT]->child[RIGHT]))
               tnode->child[RIGHT] = right_rotate(tnode->child[RIGHT]);/* the tree is right heavy and the right subtree is left heavy as  rotation to the right subtree then left rotation to the tree*/
            return left_rotate(tnode); /* the tree and the right subtree are right heavy then left rotation to the tree*/
        }
    }
    return NULL;
}

/* Operations */

/*This function return's the node that her 'key' is k*/
AVLNodePtr avl_search(AVLNodePtr tnode, int k) {
    if (tnode==NULL)
        return tnode;

    if(tnode->key==k)
        return tnode;

     if (tnode->key > k)
        return avl_search(tnode->child[LEFT], k);

     return avl_search(tnode->child[RIGHT], k);
}

/*This function insert's a new node with the key 'k' ,
  if 'k' is exist on the tree do not do anything. return a pointer to the new root*/
AVLNodePtr avl_insert(AVLNodePtr tnode, int k) {
    if (tnode==NULL) {
        return new_avl_node(k);
    }
    if (tnode->key > k) {
        tnode->child[LEFT] = avl_insert(tnode->child[LEFT], k);
    }
    if (tnode->key < k) {
        tnode->child[RIGHT] = avl_insert(tnode->child[RIGHT], k);
    }

    if (tnode->key == k)
    {
        return tnode;
    }
        tnode->height =update_height(tnode);
        tnode->size = update_size(tnode);
        return balance(tnode);

}

/*This function delete's the node that her key is 'k',
 if 'k' is not on the tree do not do anything. return a pointer to the new root*/
AVLNodePtr avl_delete(AVLNodePtr tnode, int k) {

    if (!tnode)
        return NULL;
    if (tnode->key < k){
         tnode->child[RIGHT]= avl_delete(tnode->child[RIGHT], k);
    }
    if (tnode->key > k){
      tnode->child[LEFT] = avl_delete(tnode->child[LEFT], k);
    }
    if (tnode->key == k)
    {
         AVLNodePtr temp=NULL;
        if (tnode->child[LEFT]==NULL && tnode->child[RIGHT]==NULL)
        {
            free(tnode);
            return NULL;
        }

        if (tnode->child[LEFT] == NULL && tnode->child[RIGHT]!=NULL)
        {
            temp = tnode;
            tnode = tnode->child[RIGHT];
            free(temp);
        }

        if (tnode->child[RIGHT] == NULL && tnode->child[LEFT]!=NULL )
        {
            temp = tnode;
            tnode = tnode->child[LEFT];
            free(temp);
        }

        if (tnode->child[RIGHT]!=NULL && tnode->child[LEFT]!=NULL)
        {
            temp = avl_min(tnode->child[RIGHT]);
            tnode->key = temp->key;
            tnode->child[RIGHT]= avl_delete(tnode->child[RIGHT],temp->key);
        }
    }
            tnode->size = update_size(tnode);
            tnode->height = update_height(tnode);
            return balance(tnode);
}
/*This function build's a new node that her 'key' is 'k'*/
AVLNodePtr new_avl_node(int k) {
    AVLNodePtr newNode = (AVLNodePtr)malloc(sizeof(AVLNode));
    if (newNode == NULL) {
        return NULL;
    }
    else {
        newNode->child[LEFT] = NULL;
        newNode->child[RIGHT] = NULL;
        newNode->key = k;
        newNode->height = 0;
        newNode->size = 1;
        return newNode;
    }
}
/*This function delete's the tree and release the memory*/
void delete_avl_tree(AVLNodePtr root) {
    if (root)
    {
        delete_avl_tree(root->child[LEFT]);
        delete_avl_tree(root->child[RIGHT]);
        free(root);
    }
}
/* Queries */
/*This function return's the max number that is smaller than the maximum on the tree and not exists on the tree*/
int last_missing_assistant(AVLNodePtr tnode, int max, int counter){
    if((tnode->key + getSize(tnode->child[RIGHT])) < max - counter){ /* the missing number is in the right subtree*/
        if(!tnode->child[RIGHT]){
            return (max - counter);
        }
        return last_missing_assistant(tnode->child[RIGHT], max, counter); /* search the last missing on the right subtree*/
    }
    else{ /* the last missing is in the left subtree*/
        counter += (getSize(tnode->child[RIGHT]) + 1); /* save the amount of the numbers that not missing*/

        if(!tnode->child[LEFT]){
            return (max-counter);
        }
        return last_missing_assistant(tnode->child[LEFT], max, counter);
    }
}
int last_missing( AVLNodePtr tnode ){
    int max = avl_max(tnode)->key;/* save the max value on the tree*/
    if(!tnode){
        return 1;
    }
    return last_missing_assistant(tnode, max, 0);
}

/*the assistant of avl_rank. the counter is for the amount of nodes whose smaller than k till now*/
int avl_rank_assistant(AVLNodePtr tnode, int k , int counter)
{
    if(!tnode)  /*if we reached NULL, just return what's in counter.*/
        return counter;
    if(tnode->key < k) /*tnode and it's left subtree are smaller than k, so modify counter in the call*/
        return (avl_rank_assistant(tnode->child[RIGHT],k,(getSize(tnode->child[LEFT])+ 1 + counter)));
    if(tnode->key > k)
        return (avl_rank_assistant(tnode->child[LEFT], k , counter));
    if(tnode->key==k)
        return(getSize(tnode->child[LEFT])+ 1 + counter);
    return 0;
}

/*This function return's the number of the numbers that are smaller than and equal k on the tree*/
int avl_rank(AVLNodePtr tnode, int k)
{
  return avl_rank_assistant(tnode,k,0);
}

/*This function return's the number of the numbers that are larger than and equal k on the tree*/
int avl_reversed_rank(AVLNodePtr tnode, int k)
{
  int counter=0;
  if(!tnode) /*if we reached NULL, just return what's in counter.*/
    return counter;
  if(tnode->key<k){
    counter+=avl_reversed_rank(tnode->child[RIGHT],k);
  }
  if(tnode->key>k) /*tnode and it's right subtree are smaller than k, so modify counter in the call*/
  {
      counter+=getSize(tnode->child[RIGHT])+1;
      counter+= avl_reversed_rank(tnode->child[LEFT],k);
  }
  if(tnode->key==k)
  {
      counter+=getSize(tnode->child[RIGHT])+1;
  }
   return counter;

}
/*This function return's the number of the numbers that are larger than and equal x on the tree and smaller than and equal y on the tree */
int avl_HowMany(AVLNodePtr tnode, int x, int y)
{
    int xRank, yRank;
    if(!tnode)
        return 0;
    xRank=avl_rank(tnode,x);
    yRank=avl_rank(tnode,y);
    if(avl_search(tnode,x)==NULL&&avl_search(tnode,x)==NULL) /* if x and y are not found on the tree return the subtract of rank y and rank x*/
        return (yRank-xRank);
    if(avl_search(tnode,y)!=NULL &&avl_search(tnode,x)==NULL) /* if y is found on the tree and x is not return the subtract of rank y and rank x*/
        return (yRank-xRank);
      return (yRank - xRank + 1) ;
}



