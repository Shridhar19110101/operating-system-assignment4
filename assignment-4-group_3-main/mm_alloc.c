/*
 * mm_alloc.c
 */

#include "mm_alloc.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


block_ptr HeadPtr=NULL;

block_ptr extend_heap (block_ptr last , size_t s){
	void* p=sbrk(s+sizeof(meta_block));
		
	if(!(p==(void *)-1)){ 
		
		block_ptr newBlock = (block_ptr) p;
		if(last){
			last->next=newBlock;
			
		}else{
			HeadPtr=newBlock;
		}
    //insertion
		newBlock->prev=last;
		newBlock->next=NULL;
		newBlock->size=s;
		newBlock->block=p+sizeof(meta_block);
		//last->ptr=p;
		newBlock->free=0;
		
		return newBlock;
	}
	//Return Null on error
	return NULL;
}

block_ptr get_block (void *p){
	block_ptr head=HeadPtr;
	
	while(head){
		if(head->block==p){
			return head;
		}
		head=head->next;
	}
	//null if we cant find it.
	return NULL;
	
}

block_ptr merge(block_ptr b){
	if((b->next)->free == 1){
		b->size=b->size+sizeof(meta_block)+(b->next)->size;
		b->next=(b->next)->next;
		(b->next)->prev=b;
	}
	
	if((b->prev)->free == 1){
		
		(b->prev)->next=b->next;
		(b->next)->prev=b->prev;
		(b->prev)->free=b->free;
		(b->prev)->size=(b->prev)->size + sizeof(meta_block) + b->size;
		
		return b->prev;
	}
	
	return b;
}

void split_block (block_ptr b, size_t s){
	if(b && s >= sizeof(void *)){
		if((b->size-s) >= sizeof(meta_block) + sizeof(void*) ){
			block_ptr new_block_ptr = (block_ptr) (b->block + s); //start of new block
			//inserting the new block in list
      new_block_ptr->next = b->next; 
			(new_block_ptr->next)->prev=new_block_ptr;
			b->next=new_block_ptr;
			new_block_ptr->prev=b;
			new_block_ptr->size=b->size - s - sizeof(meta_block);
			b->size = s;
			new_block_ptr->block= b->block + s + sizeof(meta_block);
			
			mm_free(new_block_ptr->block);
		}
	}
}

void* mem_copy(block_ptr old_block, block_ptr new_block){
	if(old_block && new_block){
		char * oStart= (char *) old_block->block;
		char * nStart= (char *) new_block->block;
		size_t i;
    //copying data from old block to new block
		for(i=0;i<old_block->size;i++){
			*(nStart + i)=*(oStart + i);
		}
		return new_block->block;
	}
	return NULL;
}


void* mm_malloc(size_t size) {
  //TODO: Implement malloc
  block_ptr head = HeadPtr;
  block_ptr prev = NULL;
  //searching the block list
  while(head){
    //if the block is free and it's size is equal or more than required
    if(head->free == 1 && head->size >= size){
			mm_realloc(head->block,size);
			head->free=0;
			return head->block;
			
		}else{
			prev=head;
			head=head->next;
		}
	}
//We exited the loop without returning which implies that there is no suitable space. Hence extend the break pointer.
	head = extend_heap(prev,size);
	if(!head){
		exit(EXIT_FAILURE);
	}
	return head->block;
}

void* mm_realloc(void* ptr, size_t size) {
  //TODO: Implement realloc
  block_ptr curr = get_block(ptr);
		if(curr){
			if(size > curr->size){
				void* p = mm_malloc(size);
				block_ptr new_block = get_block(p);
				if(new_block){
					p = mem_copy(curr,new_block);
					mm_free(curr->block);
					return p;
				}
			}else if(size < curr->size){
				split_block(curr,size);
				return curr->block;
			}else{
				return curr->block;
			}
		}
	return NULL;
}

void mm_free(void* ptr) {
  //TODO: Implement free
  block_ptr curr=get_block(ptr);
	
	if(curr){
		if(curr->next == NULL){ //last block
			if(curr->prev){ //not the first block
				(curr->prev)->next=NULL;
			}else{ //first block
				HeadPtr=NULL;
			}
      //decrementing the break point
			sbrk(-(curr->size + sizeof(meta_block)));
		}else{ //if not last block
			curr->free=1;
			merge(curr);
		}
	}
}
