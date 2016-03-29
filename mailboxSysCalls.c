// Name: Kyle Fritz
// File: mailboxSysCalls.c
// Date Created: 3/26/16
// Description: New sys calls for managing mailboxes for 421 Proj1.

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <include/asm/spinlock.h>

static int numMailboxes = 0;


struct mbox
{
	unsigned long ID;
	char* messages[];
	int numMessages = 0;
};

// DONE! Array of mailboxes
struct mbox Mboxes[];

void bringMessagesUp(struct mbox* currMbox){
	int i = 0;
	while (i < currMbox->numMessages){
		if(i == (currMbox->numMessages - 1)){
			currMbox->messages[i] = NULL;
		}
		else{
			currMbox->messages[i] = currMbox->messages[i+1];
		}

		i++;
	}
}

// DONE! Encrypts the message and returns it
unsigned char* encrypt(unsigned char* msg, unsigned long len, unsigned long key){
	extra = 4 - (len%4); // Extra zeroes needed

	int i = 0;
	while(i < (len + extra){ // encrypt
		if(i >= len){ // if longer than len of message, pad with zeroes 
			msg[i] = 0;
		}
		else{
			msg[i] = msg[i] ^ key[i]; // xor
		}
		i++;
	}

	i = len;
	while(i < (len + extra)){ // get rid of padded zero bits
		msg[i] = NULL;
		i++;
	}

	return msg;
}

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

// DONE! Creates a new mailbox if it can
asmlinkage long create_mbox_421(unsigned long id){
	printk("In create_mbox_421\n");

	if (checkIfIdAvailable(id) == false){ // return error if id already taken
		return -EINVAL;
	}

	struct mbox *newMbox = (struct mbox*) kmalloc(sizeof(struct mbox));
	newMbox->ID = id;
	int i = 0;
	while(i <= numMailboxes){
		if(Mboxes[i] != NULL){
			Mboxes[i] = newMbox;
		}
		i++;
	}
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

// DONE! Returns numMailboxes
asmlinkage long count_mbox_421(void){
	printk("In count_mbox_421\n");
	return numMailboxes;
}

// DONE! Returns an array of a max of k IDs in mboxes
asmlinkage long list_mbox_421(unsigned long *mboxes, unsigned long k){
	printk("In list_mbox_421\n");

	if(k >= numMailboxes){ // if k too big, scale it down to numMailboxes
		k = numMailboxes;
	}

	int i = 0;
	int j = 0;
	while(i < k){
		if(Mboxes[j]->ID != NULL){
			mboxes[i] = Mboxes[j]->ID; // parameter array becomes
			i++;
			j++;
		}
		else{
			j++;
		}
	}

	return mboxes;
}

// DONE! appends the msg to the mbox specified to id, returns size of msg (n bytes)
asmlinkage long send_msg_421(unsigned long id, unsigned char *msg, unsigned long n, unsigned long key){
	printk("In send_msg_421\n");

	int i = 0;
	int count = 0;
	while(Mboxes[i]->ID != id){
		if(Mboxes[i]->ID != NULL){
			count++;
		}
		if(count >= numMailboxes){
			return -EINVAL;
		}
	}
	struct mbox *currMbox = Mboxes[i];

	msg = encrypt(msg, n, key);

	currNumMessages = currMbox->numMessages; // the mbox's current number of messages
	currMbox->messages[currNumMessages] = msg; // set next message

	currMbox->numMessages +=1; // increment numMessages

	return n; // message length (i.e. bytes)
}

asmlinkage long recv_msg_421(unsigned long id, unsigned char *msg, unsigned long n, unsigned long key){
	printk("In recv_msg_421\n");

	int i = 0;
	int count = 0;
	while(Mboxes[i]->ID != id){
		if(Mboxes[i]->ID != NULL){
			count++;
		}
		if(count >= numMailboxes){
			return -EINVAL;
		}
	}
	struct mbox *currMbox = Mboxes[i];

	msg = encrypt(msg, n, key); // decrypt it w/ encrpyt (works the same w/ xor)

	msg = currMbox->messages[0]; // first message

	bringMessagesUp(currMbox); // make all subsequent messages go up one

	currMbox->numMessages -=1; // decrements numMessages

	len = len(msg)

	return n;
}

// DONE! finds the id then returns numMessages for that mailbox
asmlinkage long count_msg_421(unsigned long id){
	printk("In count_msg_421\n");

	int i = 0;
	int count = 0;
	while(Mboxes[i]->ID != id){
		if(Mboxes[i]->ID != NULL){
			count++;
		}
		if(count >= numMailboxes){
			return -EINVAL;
		}
	}
	struct mbox *currMbox = Mboxes[i];

	return currMbox->numMessages;
}
