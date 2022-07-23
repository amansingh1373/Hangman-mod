#include<iostream>
#include<fstream>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#define MAX_length 28
#define row 50000
#define column 29
#define MAX 100

using namespace std;


struct node
{
	char ch;
	int count;
	node *next;
};

int lives=5;

node *temp_front=NULL,*temp_rear=NULL;

void create_alphalist(node **front,node **rear)
{
	char letter='a';
		
	for(int i=0;i<26;i++)
	{
		node *p=new node();
		p->ch=letter;
		p->count=0;
		
		if(*rear==NULL)
		{
			*front=*rear=p;
		}
		else
		{
			(*rear)->next=p;
			*rear=p;
		}
		(*rear)->next=NULL;
		letter++;
	}
}

void delete_alphalist(node **front,node **rear)
{
	node *temp=NULL;
	while(*front!=NULL)
	{
		temp=*front;
		*front=(*front)->next;
		free(temp);
	}
	*rear=NULL;
}


void display_alphalist(node *front)
{
	while(front!=NULL)
	{
		cout<<front->ch<<":"<<front->count<<" ";
		front=front->next;	
	}	
	cout<<endl;
}

void sort_linkedlist(node *front)
{
	node *temp1=front,*temp2=front;
	int swap,i=1;
	char swap_ch;
	
	while(temp1!=NULL)
	{
		temp2=front;
		while((temp2->next)->next!=NULL)
		{
			if(temp2->count<(temp2->next)->count)
			{
				swap_ch=temp2->ch;
				temp2->ch=(temp2->next)->ch;
				(temp2->next)->ch=swap_ch;
				
				swap=temp2->count;
				temp2->count=temp2->next->count;
				(temp2->next)->count=swap;
			}
			temp2=temp2->next;
		}
		temp1=temp1->next;
	}
}

void create_usedseries(char temp_table[row/6][column],int x,int length)
{
	node *temp=NULL;
	for(int i=0;i<x;i++)
	{
		for(int j=0;j<length;j++)
		{
			temp=temp_front;
			while(temp!=NULL)
			{
				if(temp->ch==temp_table[i][j])
				{
					temp->count++;
					break;
				}
				temp=temp->next;
			}
		}
	}
	sort_linkedlist(temp_front);
}

void mod(char table1[row][column],int table2[MAX_length][7],char str[MAX],int *pos_occupied,int length)
{
	int flag=1,x=0,pos;//x is the max row in the temp table

	char temp_table[row/6][column];
	
	
	for(int i=table2[length-1][0];i<=table2[length-1][1];i++)
	{
		for(int j=0;j<length;j++)
		{
			if(*(pos_occupied+j)==1)
			{
				if(table1[i][j]!=str[j])
				{
					flag=0;
					break;
				}
			}
		}
		if(flag==1)
		{
			for(int k=0;k<length;k++)
			{
				temp_table[x][k]=table1[i][k];
				pos=k;
			}
			x++;
			temp_table[x-1][pos+1]='\0';
		}
		flag=1;
	}
	create_alphalist(&temp_front,&temp_rear);
	create_usedseries(temp_table,x,length);
}


int hangman(char table1[row][column],int table2[MAX_length][7])
{
	ifstream fin;
	char word[MAX],str[MAX];
	int *pos_occupied;
	char token;
	node *front=NULL,*rear=NULL,*temp=NULL,*del=NULL;
	int length=0,flag=0,fail_counter=0,pass_flag=0,firsttime=0,prelife=5,repeat=0,won=1,victory=0,num=1;
	
//	for(int i=0;i<length;i++)
//	{
//		*(pos_occupied+i)=0;
//	}
	
	fin.open("50000_words.txt",ios::in);
	int hello=1;
		while(hello==1)
		{
			hello=0;
//			fin>>word;
			strcpy(word,"administer");
			length=strlen(word);
			
//			cout<<word<<endl;
			for(int i=0;i<length;i++)
			{
				str[i]='_';
			}
			create_alphalist(&front,&rear);
			
			pos_occupied=new int[length];
			while(lives!=0)
			{
				cout<<endl;
				for(int i=0;i<length;i++)
				{
					cout<<str[i]<<" ";
				}
				cout<<endl<<"lives:"<<lives;
				above:
				if(firsttime==0&&pass_flag==0)
				{
					token=(char)table2[length-1][2+fail_counter];
					firsttime=1;
				}
				else if(lives!=prelife||repeat==1)
				{
					temp=temp_front;
					for(int i=0;i<=fail_counter;i++)
					{
						token=temp->ch;
						temp=temp->next;
					}
				}
				else
				{
					mod(table1,table2,str,pos_occupied,length);
					temp=temp_front;
					token=temp->ch;
				}
				
				
				temp=front;
				while(temp!=NULL)
				{
					if(temp->ch==token)
					{
						if(temp->count==1)
						{
							fail_counter++;
							repeat=1;
							goto above;
						}
						else
						{
							repeat=0;
							temp->count++;
						}
						break;
					}
					temp=temp->next;
				}
				
				cout<<"\nguessed letter:"<<token<<endl;
				
				cout<<endl;
				display_alphalist(front);
				cout<<endl;
				
				for(int i=0;i<length;i++)
				{
					if(word[i]==token)
					{
						str[i]=token;
						*(pos_occupied+i)=1;
						flag=1;
					}
					else if(*(pos_occupied+i)!=1)
					{
						*(pos_occupied+i)=0;
					}
				}
				if(flag==1)
				{
//					cout<<"\nflag=1\n";
					for(int i=0;i<length;i++)
					{
						if(word[i]!=str[i])
						{
							won=0;
						}
					}
					/*won game condition*/
					if(won==1)
					{
//						cout<<"you won";
						victory++;
						break;
					}
					pass_flag=1;
					delete_alphalist(&temp_front,&temp_rear);
					fail_counter=0;
					prelife=lives;
					flag=0;
				}
				else
				{
//					cout<<"\nflag=0\n";
					prelife=lives;
					lives--;
					fail_counter++;
					firsttime=0;
				}
				won=1;
			}
//			if(lives==0)
//			{
//				cout<<num<<" "<<word<<endl;
//				num++;
//			}
			pass_flag=0;
			firsttime=0;
			fail_counter=0;
			prelife=5;
			lives=5;
			repeat=0;
			for(int i=0;i<length;i++)
			{
				*(pos_occupied+i)=0;
			}
			delete[] pos_occupied;
			for(int i=0;i<length;i++)
			{
				str[i]='_';
			}
			delete_alphalist(&temp_front,&temp_rear);
			delete_alphalist(&front,&rear);
		}
	fin.close();
	return victory;
}

int create_tables(char table1[row][column],int table2[MAX_length][7],int wlength,int end)
{
	ifstream fin;
	string word;
	node *front=NULL,*rear=NULL,*temp_LL=NULL;
	int length,flag=0,temp=end;
	
	create_alphalist(&front,&rear);
	
	fin.open("50000_words.txt",ios::in);
		while(fin.eof()!=1)
		{
			fin>>word;
			temp_LL=front;
			
			length=word.length();
			if(length==wlength)
			{
				flag=1;
				for(int i=0;i<length;i++)
				{
					
					table1[end][i]=word[i];
					
					temp_LL=front;
					while(temp_LL!=NULL)
					{
						if(word[i]==(temp_LL->ch))
						{
							temp_LL->count++;
							break;
						}
						temp_LL=temp_LL->next;
					}
				}
				table1[end][length]='\0';
				end++;
			}
		}
	fin.close();
	
	sort_linkedlist(front);
	
	temp_LL=front;
	
	if(flag==1)
	{
		table2[wlength-1][1]=end-1;
		table2[wlength-1][0]=temp;
		for(int i=2;i<=7;i++)
		{ 
			table2[wlength-1][i]=(int)temp_LL->ch;
			temp_LL=temp_LL->next;	
		}
	}
	else
	{
		for(int i=0;i<8;i++)
		{
			table2[wlength-1][i]=0;
		}
	}
		
	return end;	
}

int main()
{
	cout<<"\n\nSTART\n\n";
	getch();
	char table1[row][column];
	int table2[MAX_length][7],end=0,victory; 
	
	for(int i=1;i<=MAX_length;i++)
	{
		end=create_tables(table1,table2,i,end);
	}
	 
	/*if you wantt to see the tables*/
//	for(int i=0;i<50000;i++)
//	{
//		cout<<table1[i];
//		cout<<endl;
//	}
//	
//	for(int i=0;i<28;i++)
//	{
//		cout<<i+1<<" ";
//		for(int j=0;j<7;j++)
//		{
//			cout<<table2[i][j]<<" ";
//		}
//		cout<<endl;
//	}
	
	victory=hangman(table1,table2);
//	cout<<"victory:"<<victory<<" "<<"loss:"<<50000-victory;
	cout<<"\n\nEND\n\n";
	return 0;
}
