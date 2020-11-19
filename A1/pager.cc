#include <iostream>
#include "vm_pager.h"
#include <string>
#include <string.h>
#include <queue>
#include <cstdlib>
using namespace std;
struct page
{
	bool dirty;
	bool zero;
	bool resident;
	bool read;
	bool write;
	bool reference;
	int disk;
	int pid;
};
struct process
{
	pid_t processID;
	page_table_t pageTable;
	page** pages;
	int pageSize;
};

struct indexedPage
{
	int index;
	page *p;
};

process current;
int memory[1024];
int memorySize = 0;
int disk[1024];
int diskSize = 0;
struct process** processes;
int processSize = 0;
queue<indexedPage*> count;

extern void vm_init(unsigned int memory_pages, unsigned int disk_blocks)
{	
	for(int i = 0; i < memory_pages; i++)
	{	
		memory[memorySize] = i;
		memorySize++;
	}
	for(int i = 0; i < disk_blocks; i++)
	{
		disk[diskSize] = i;
		diskSize++;
	}
//cout<<"disk fine"<<endl;
}

extern void vm_create(pid_t pid)
{
cout<<"create fine"<<endl;
	struct process *newProcess = new process;
	newProcess->processID = pid;
	int i = 0;
	while(i < VM_ARENA_SIZE / VM_PAGESIZE)
	{
		newProcess->pageTable.ptes[i].ppage = 10000;
		newProcess->pageTable.ptes[i].read_enable = 0;
		newProcess->pageTable.ptes[i].write_enable = 0;
		i++;
	}
	processes[processSize] = newProcess;
	processSize++;
}

extern void vm_switch(pid_t pid)
{
cout<<"switch fine"<<endl;
	for(int i = 0; i < processSize; i++)
	{
		if(processes[i]->processID == pid)
		{
			current = *processes[i];
			break;
		}
	}
	page_table_base_register = &current.pageTable;
}

extern int vm_fault(void *addr, bool write_flag)
{
cout<<"fault fine"<<endl;
	long index = (long)addr - (long)VM_ARENA_BASEADDR / VM_PAGESIZE;
	page_table_entry_t PTE = current.pageTable.ptes[index];

	if(PTE.ppage == 10000)
	{
		if(memorySize == 0)
		{
			while(count.front()->p->reference == 1)
			{
				
				int in = count.front()->index;
				current.pageTable.ptes[in].read_enable = 0;
				current.pageTable.ptes[in].write_enable = 0;
				count.front()->p->read = 0;
				count.front()->p->write = 0;
				count.front()->p->reference = 0;
				count.push(count.front());
				count.pop();
			}
			if(count.front()->p->dirty)
			{
				disk_write(count.front()->p->disk, current.pageTable.ptes[count.front()->index].ppage);
				count.front()->p->dirty = 0;
				count.front()->p->resident = 0;
				count.front()->p->read = 0;
				count.front()->p->write = 0;
				count.front()->p->reference = 0;
			}
		}
		
		int mem = memory[memorySize - 1];
		memorySize--;
		current.pageTable.ptes[index].ppage = mem;
		current.pages[index]->resident = 1;
		if(current.pages[index]->zero != 1)
		{
			disk_read(current.pages[index]->disk, current.pageTable.ptes[index].ppage);
		}
		else
		{
			memset((char*) pm_physmem + (mem * VM_PAGESIZE), 0, VM_PAGESIZE);
		}
		
		indexedPage *ip;
		ip->index = index;
		ip->p = current.pages[index];
		count.push(ip);
		//return -1;
	}
	if(write_flag == true || current.pages[index]->dirty == 1)
	{
		if(PTE.read_enable == 0 || PTE.write_enable == 0)
		{
			current.pages[index]->read = 1;
			current.pages[index]->write = 1;
			current.pages[index]->dirty = 1;
			current.pages[index]->reference = 1;
			current.pages[index]->zero = 0;
			current.pageTable.ptes[index].read_enable = 1;
			current.pageTable.ptes[index].write_enable = 1;
			return 0;
		}
	}
	if(write_flag == false && PTE.read_enable == 0)
	{
		current.pageTable.ptes[index].read_enable = 1;
		current.pages[index]->read = 1;
		current.pages[index]->reference = 1;
		return 0;
	}
	return -1;
}

extern void vm_destroy()
{
cout<<"destroy fine"<<endl;
	int i = 0;
	while(i < current.pageSize)
	{
		page* t = current.pages[i];
		if(t->resident)
		{
			memory[memorySize] = current.pageTable.ptes[i].ppage;
			memorySize++;
		}
		disk[diskSize] = t->disk;
		diskSize++;
		free(current.pages[i]);
		i++;
	}
	int j = count.size();
	while(i > 0)
	{
		if(count.front()->p->pid != 0)
		{
			count.push(count.front());
			count.pop();
		}
		else
		{
			count.pop();
		}
	}
	
}

extern void * vm_extend()
{
cout<<"extend fine"<<endl;
	if(diskSize != 0)
	{
		int location = disk[diskSize];
		diskSize--;
		struct page* p = new page;
		p->dirty = false;
		p->zero = true;
		p->resident = false;
		p->read = false;
		p->write = false;
		p->reference = false;
		p->pid = (int)current.processID;
		p->disk = location;

		current.pages[current.pageSize] = p;
		current.pageSize++;
		void* address = (void *)VM_ARENA_BASEADDR + ((current.pageSize - 1) * VM_PAGESIZE);
		//int a  = VM_ARENA_BASEADDR + (current.pageSize - 1) * VM_PAGESIZE;
		return address;
	}
}

extern int vm_syslog(void *message, unsigned int len)
{
cout<<"syslog fine"<<endl;
	string str = "";
	bool c1 = (unsigned long)message + len > (unsigned long)VM_ARENA_BASEADDR + (unsigned long)VM_ARENA_SIZE;
	bool c2 = (long)message < (long)VM_ARENA_BASEADDR;
	if(len == 0 || c1 || c2)
	{
		return -1;
	}
	while(len > 0)
	{
		long offset = (long)message % (long)VM_PAGESIZE;
		long temp = VM_PAGESIZE - offset;
		long index = (long)message / (long)VM_PAGESIZE - (long)VM_ARENA_BASEADDR / (long)VM_PAGESIZE;
		if(current.pages[index]->resident == 0)
		{
			current.pageTable.ptes[index].write_enable ==0;
			current.pageTable.ptes[index].read_enable ==0;
			vm_fault(message, 0);
		}
		while(offset != VM_PAGESIZE)
		{
			long processAddress = offset + VM_PAGESIZE * current.pageTable.ptes[index].ppage;
			str += ((char*)pm_physmem)[processAddress];
			offset++;
		}
	}
	cout<<"syslog \t\t\t"<<str<<endl;
	return 0;
}
