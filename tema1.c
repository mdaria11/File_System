#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

//structuri

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

typedef struct Nod {
	char *name;
	struct Nod *next;
} Nod;

typedef struct Stiva {
	struct Nod *head;
} Stiva;

//functii

void push(char *name, Stiva *s)
{
	Nod *nou;
	int len;

	len=strlen(name);

	nou=malloc(sizeof(Nod));
	nou->name=malloc(sizeof(char)*len+1);
	snprintf(nou->name, len+1, "%s", name);

	nou->next=s->head;
	s->head=nou;
}

char* pop(Stiva *s)
{
	Nod *flag;
	int len;
	char *nume;

	flag=s->head;

	len=strlen(flag->name);
	nume=malloc(sizeof(char)*len+1);
	snprintf(nume, len+1, "%s", flag->name);

	s->head=flag->next;

	free(flag->name);
	free(flag);

	return nume;

}

void touch (Dir* parent, char* name)
{
	File *fisiernou, *flag;
	int len;
	Dir *flag1;

	flag=parent->head_children_files;
	flag1=parent->head_children_dirs;

	while(flag!=NULL)
	{
		if(strcmp(flag->name, name)==0)
		{
			printf("File already exists\n");

			return;
		}

		flag=flag->next;
	}

	while(flag1!=NULL)
	{
		if(strcmp(flag1->name, name)==0)
		{
			printf("File already exists\n");

			return;
		}

		flag1=flag1->next;
	}

	fisiernou=malloc(sizeof(File));
	len=strlen(name);
	fisiernou->name=malloc(sizeof(char)*len+1);
	snprintf(fisiernou->name, len+1, "%s", name);
	fisiernou->parent=parent;
	fisiernou->next=NULL;

	flag=parent->head_children_files;

	if(flag==NULL)
	{
		parent->head_children_files=fisiernou;

		return;
	}
	else
	{
		while(flag->next!=NULL)
		{
			flag=flag->next;
		}

		flag->next=fisiernou;

		return;
	}
}

void mkdir (Dir* parent, char* name)
{
	Dir *flag1, *dirnou;
	File *flag;
	int len;

	flag=parent->head_children_files;
	flag1=parent->head_children_dirs;

	while(flag!=NULL)
	{
		if(strcmp(flag->name, name)==0)
		{
			printf("Directory already exists\n");

			return;
		}

		flag=flag->next;
	}

	while(flag1!=NULL)
	{
		if(strcmp(flag1->name, name)==0)
		{
			printf("Directory already exists\n");

			return;
		}

		flag1=flag1->next;
	}

	dirnou=malloc(sizeof(Dir));
	len=strlen(name);
	dirnou->name=malloc(sizeof(char)*len+1);
	snprintf(dirnou->name, len+1, "%s", name);
	dirnou->parent=parent;
	dirnou->head_children_files=NULL;
	dirnou->head_children_dirs=NULL;
	dirnou->next=NULL;

	flag1=parent->head_children_dirs;

	if(flag1==NULL)
	{
		parent->head_children_dirs=dirnou;

		return;
	}
	else
	{
		while(flag1->next!=NULL)
		{
			flag1=flag1->next;
		}

		flag1->next=dirnou;

		return;
	}

}

void ls (Dir* parent)
{
	Dir *flag;
	File *flag1;

	flag=parent->head_children_dirs;

	while(flag!=NULL)
	{
		printf("%s\n", flag->name);

		flag=flag->next;
	}

	flag1=parent->head_children_files;

	while(flag1!=NULL)
	{
		printf("%s\n", flag1->name);

		flag1=flag1->next;
	}
}

void rm (Dir* parent, char* name)
{
	File *flag, *flag1;

	flag=parent->head_children_files;
	flag1=flag;

	while(flag!=NULL)
	{
		if(strcmp(name, flag->name)==0)
		{
			if(flag==parent->head_children_files)
			{
				parent->head_children_files=flag->next;
			}
			else
			{
				flag1->next=flag->next;
			}

			free(flag->name);
			free(flag);

			return;
		}

		flag1=flag;
		flag=flag->next;
	}

	printf("Could not find the file\n");
}

void stop (Dir* target)
{
	free(target->name);

	File *flag, *flag1;
	Dir *targ, *targ1;

	flag=target->head_children_files;
	flag1=flag;

	while(flag!=NULL)
	{
		free(flag->name);
		flag1=flag->next;
		free(flag);
		flag=flag1;
	}

	targ=target->head_children_dirs;

	while(targ!=NULL)
	{
		targ1=targ->next;

		stop(targ);

		targ=targ1;
	}

	free(target);
}

void rmdir (Dir* parent, char* name)
{
	Dir *flag, *flag1;

	flag=parent->head_children_dirs;
	flag1=flag;

	while(flag!=NULL)
	{
		if(strcmp(name, flag->name)==0)
		{
			if(flag==parent->head_children_dirs)
			{
				parent->head_children_dirs=flag->next;
			}
			else
			{
				flag1->next=flag->next;
			}

			stop(flag);

			return;
		}

		flag1=flag;
		flag=flag->next;
	}

	printf("Could not find the dir\n");
}

void cd(Dir** target, char *name)
{
	Dir *flag;

	if(strcmp(name, "..")==0)
	{
		if((*(target))->parent!=NULL)
		{
			*(target)=(*(target))->parent;
		}

		return;
	}

	flag=(*(target))->head_children_dirs;

	while(flag!=NULL)
	{
		if(strcmp(name, flag->name)==0)
		{
			(*(target))=flag;

			return;
		}

		flag=flag->next;
	}

	printf("No directories found!\n");
}

char *pwd (Dir* target)
{
	char *cale, *nume, *aux;
	Stiva *s;
	Dir *flag;
	int len, len1;

	s=malloc(sizeof(Stiva));
	s->head=NULL;

	flag=target;

	while(flag!=NULL)
	{
		push(flag->name, s);

		flag=flag->parent;
	}

	cale=malloc(sizeof(char)*1);
	cale[0]='\0';

	while(s->head!=NULL)
	{
		aux=malloc(sizeof(char)*2);

		nume=pop(s);

		len=strlen(nume);

		snprintf(aux, 2, "%s", "/");

		aux=realloc(aux, sizeof(char)*(2+len));

		strncat(aux, nume, len+2);

		len1=strlen(cale);

		cale=realloc(cale, sizeof(char)*(len1+len+2));

		strncat(cale, aux, len1+len+2);

		free(nume);
		free(aux);

	}

	free(s);

	return cale;

}

void tree (Dir* target, int level)
{
	Dir *flag;
	File *flag1;

	flag=target->head_children_dirs;

	while(flag!=NULL)
	{
		for(int i=0; i<level; i++)
		{
			printf("    ");
		}

		printf("%s\n", flag->name);
		tree(flag, level+1);
		flag=flag->next;
	}

	flag1=target->head_children_files;

	while(flag1!=NULL)
	{
		for(int i=0; i<level; i++)
		{
			printf("    ");
		}

		printf("%s\n", flag1->name);
		flag1=flag1->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname)
{
	File *flag, *aux;
	Dir *flag1, *aux1, *flag2;
	int len;

	flag=parent->head_children_files;

	while(flag!=NULL)
	{
		if(strcmp(oldname, flag->name)==0)
		{
			aux=parent->head_children_files;
			aux1=parent->head_children_dirs;

			while(aux!=NULL)
			{
				if(strcmp(newname, aux->name)==0)
				{
					printf("File/Director already exists\n");

					return;
				}

				aux=aux->next;
			}

			while(aux1!=NULL)
			{
				if(strcmp(newname, aux1->name)==0)
				{
					printf("File/Director already exists\n");

					return;
				}

				aux1=aux1->next;
			}


			rm(parent, oldname);
			touch(parent, newname);

			return;
		}

		flag=flag->next;
	}


	flag1=parent->head_children_dirs;
	flag2=flag1;

	while(flag1!=NULL)
	{
		if(strcmp(oldname, flag1->name)==0)
		{
			aux1=parent->head_children_dirs;
			aux=parent->head_children_files;

			while(aux1!=NULL)
			{
				if(strcmp(newname, aux1->name)==0)
				{
					printf("File/Director already exists\n");

					return;
				}

				aux1=aux1->next;
			}

			while(aux!=NULL)
			{
				if(strcmp(newname, aux->name)==0)
				{
					printf("File/Director already exists\n");

					return;
				}

				aux=aux->next;
			}

			len=strlen(newname);
			flag1->name=realloc(flag1->name, sizeof(char)*len+1);
			snprintf(flag1->name, len+1, "%s", newname);

			if(flag1==parent->head_children_dirs)
			{
				if(flag1->next!=NULL)
				{
					parent->head_children_dirs=flag1->next;
					aux1=parent->head_children_dirs;

					while(aux1->next!=NULL)
					{
						aux1=aux1->next;
					}

					aux1->next=flag1;
					flag1->next=NULL;

					return;
				}
				else
				{
					return;
				}
			}
			else
			{
				flag2->next=flag1->next;
				aux1=parent->head_children_dirs;

				while(aux1->next!=NULL)
				{
					aux1=aux1->next;
				}

				aux1->next=flag1;
				flag1->next=NULL;

				return;
			}

		}

		flag2=flag1;
		flag1=flag1->next;
	}

	printf("File/Director not found\n");
}



int main ()
{
	char *line, *comanda, *argument1, *c, *copie, *argument2;
	Dir *root, *dircurent;
	int lenlinie;

	//initializare director home - root

	root=malloc(sizeof(Dir));
	root->name=malloc(sizeof(char)*5);
	snprintf(root->name, 5, "%s", "home");
	root->parent=NULL;
	root->head_children_dirs=NULL;
	root->head_children_files=NULL;
	root->next=NULL;

	//dircurent reprezinta directorul curent in fiecare moment(la inceput e "home")
	dircurent=root;

	//citire comenzi
	line= malloc(sizeof(char)*MAX_INPUT_LINE_SIZE);

	while(fgets(line, MAX_INPUT_LINE_SIZE, stdin))
	{
		lenlinie=strlen(line);
		line=realloc(line, sizeof(char)*lenlinie+1);

		if(strcmp(line, "stop\n")==0)
		{
			stop(root);

			break;
		}
		else
		{
			int arg=0, argx=0;

			c=line;

			while(*(c)!='\0')
			{
				if(*(c)==' ')
				{
					arg=1;
					break;
				}

				c++;
			}

			if(arg==1)
			{
				int len=0,len1=0;

				copie=line;

				while(copie!=c)
				{
					len++;
					copie++;
				}

				comanda= malloc(sizeof(char)*(len+1));
				snprintf(comanda, len+1, "%s", line);
				c++;
				copie=c;

				while(*copie!='\0')
				{
					if(*copie==' ')
					{
						argx=1;
						break;
					}

					copie++;
				}

				if(argx==1) //caz comanda mv
				{

					int len2=0;

					copie=c;

					while(*copie!=' ')
					{
						len1++;
						copie++;
					}

					argument1= malloc(sizeof(char)*len1+1);
					snprintf(argument1, len1+1, "%s", c);
					copie++;
					c=copie;

					while(*(copie)!='\0')
					{
						len2++;
						copie++;
					}

					argument2= malloc(sizeof(char)*len2);
					snprintf(argument2, len2, "%s", c);

					//Apelare mv

					mv(dircurent, argument1, argument2);

					free(argument2);

				}
				else //caz argument(touch, mkdir, rm, rmdir, cd)
				{
					copie=c;

					while(*(copie)!='\0')
					{
						len1++;
						copie++;
					}

					argument1= malloc(sizeof(char)*len1);
					snprintf(argument1, len1, "%s", c);

					//APELARE COMENZI

					if(strcmp(comanda, "touch")==0) //comanda touch
					{
						touch(dircurent, argument1);
					}
					if(strcmp(comanda, "mkdir")==0) //comanda mkdir
					{
						mkdir(dircurent, argument1);
					}
					if(strcmp(comanda, "cd")==0) //comanda cd
					{
						cd(&dircurent, argument1);
					}
					if(strcmp(comanda, "rm")==0) //comanda rm
					{
						rm(dircurent, argument1);
					}
					if(strcmp(comanda, "rmdir")==0) //comanda rmdir
					{
						rmdir(dircurent, argument1);
					}

				}

				free(comanda);
				free(argument1);
			}
			else //doar comanda (ls, tree, pwd)
			{
				int len=0;

				len=strlen(line);
				comanda= malloc(sizeof(char)*len);
				snprintf(comanda, len, "%s", line);

				//APELARE COMENZI

				if(strcmp(comanda, "ls")==0) //comanda ls
				{
					ls(dircurent);
				}
				if(strcmp(comanda, "tree")==0) //comanda tree
				{
					tree(dircurent, 0);
				}
				if(strcmp(comanda, "pwd")==0) //comanda pwd
				{
					char *cale;

					cale=pwd(dircurent);
					printf("%s\n", cale);

					free(cale);
				}

				free(comanda);
			}
		}

		line=realloc(line, sizeof(char)*MAX_INPUT_LINE_SIZE);
	}

	free(line);

	return 0;
}
