// Name: Kyle Fritz
// File: mailboxSysCalls.c
// Date Created: 3/26/16
// Description: New sys calls for managing mailboxes for 421 Proj1.

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/list.h>

static int numMailboxes = 0;


struct mbox
{
	unsigned long ID;
	char* messages[];
	int numMessages = 0;
};

// DONE! Array of mailboxes
struct mbox Mboxes[];

// DONE! Checks if the id given is still available
bool checkIfIdAvailable(unsigned long id){
	int i = 0;
	while(Mboxes[i] != NULL){
		if (Mboxes[i]->ID == id){
			return false;
		}
		i++;
	}
	return true;
}

// DONE! Finds the mailbox you need 
struct mbox* findMbox(unsigned long id){ 
	int i = 0;
	while(Mboxes[i]->ID != id){
		i++;
	}
	return Mboxes[i];
}

asmlinkage long create_mbox_421(unsigned long id){
	printk("In create_mbox_421\n");
	if (numMailboxes == 0){
		struct mbox *temp = NULL;
		struct mbox *head = (struct mbox*) kmalloc(sizeof(struct mbox));
		head->ID = id;
		temp = head;

		return 0;
	}
	else if (checkIfIdAvailable(id) == false){
		return -EINVAL;
	}

	while(temp->next != NULL){
		temp = temp->next;
	}
	struct mbox *newMbox // new mbox to add to end of list
	newMbox->ID = id;
	temp->next = newMbox;
	temp = head;

	numMailboxes++; // inc number of mailboxes
	return 0;
}

// DONE! Removes the specified mailbox if possible
asmlinkage long remove_mbox_421(unsigned long id){
	printk("In remove_mbox_421\n");
	if(checkIfIdAvailable(id) == false) { // If ID actually exists
		struct mbox* curr = findMbox(id);
		if(curr->numMessages == 0){
			curr = NULL;
			kfree(curr);
			numMailboxes--;
			return 0;
		}
	}
	return -EINVAL;
}

asmlinkage long count_mbox_421(void){
	printk("In count_mbox_421\n");
	return numMailboxes;
}
asmlinkage long list_mbox_421(unsigned long *mboxes, unsigned long k){
	printk("In list_mbox_421\n");
	return -EINVAL;
}
asmlinkage long send_msg_421(unsigned long id, unsigned char *msg, unsigned long n, unsigned long key){
	printk("In send_msg_421\n");
	return -EINVAL;
}
asmlinkage long recv_msg_421(unsigned long id, unsigned char *msg, unsigned long n, unsigned long key){
	printk("In recv_msg_421\n");
	return -EINVAL;
}
asmlinkage long count_msg_421(unsigned long id){
	printk("In count_msg_421\n");

	return -EINVAL;
}
