#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int i=0;
typedef struct Node Node;
struct Node {
   char *key;
   struct Node* left,*right;
};

Node * constructNode(char * str){
  Node * p=malloc(sizeof(Node));
  p->key=(char *)malloc(strlen(str)+1);
  strcpy(p->key,str);
  p->left=p->right=NULL;
  return p;
}

Node *decodeline(char *line)
{
    char *name=strtok(line," \n");
    return constructNode(name);
}

Node**load()
{
    FILE *fn;
    char buff[2000];
    char c;
    int j;
    fn=fopen("Dictionary.txt","r");
    if(!fn)
        printf("cannot open file");
        i=1;

    for(c=getc(fn); c!=EOF; c=getc(fn))
    {
        if (c=='\n')
            i=i+1;
    }
    fseek(fn,0L,SEEK_SET);
    Node **info=malloc(i*sizeof(Node*));
    for(j=0; j<i; j++)
    {
        if(!fgets(buff,sizeof(buff),fn))
            break;
        info[j]=decodeline(buff);
    }
    fclose(fn);
    return info;
}

Node* insert(Node* node, char *key)
{
    if (node == NULL)
        return(constructNode(key));
    if (strcasecmp(key ,node->key)<0)
        node->left  = insert(node->left, key);
    else if (strcasecmp(key ,node->key)>0)
        node->right = insert(node->right, key);

    return node;
}

int height(Node *n)
{
    int left;
    int right;
    if(n==NULL)
    {
        return -1;
    }else
    {
        left=1+height(n->left);
        right=1+height(n->right);
        if(left>right)
        {
            return left;
        }else
        {
            return right;
        }
    }
}

Node *search(Node *n,char *str)
{
    while(n)
    {
        if(strcasecmp(str,n->key)==0)
        {
            return n;
        }else if(strcasecmp(str,n->key)<0)
        {
            n=n->left;
        }else
        {
            n=n->right;
        }
    }
    return NULL;
}

Node *searchbeforenull(Node *n,char *str) //get node where strcasecmp = 1 and n->right==NULL or strcasecmp=-1 and n->left==NULL
{
    while(n)
    {
        if(strcasecmp(str,n->key)==0)
        {
            return n;
        }else if(strcasecmp(str,n->key)<0)
        {
            if(n->left==NULL)
                return n;
            n=n->left;
        }else
        {
            if(n->right==NULL)
                return n;
            n=n->right;
        }
    }
    return NULL;
}

Node *findmin(Node *root)
{   if(root==NULL)
    {
        return NULL;
    }
    if(root->left ==NULL)
    {
        return root;
    }else
    {
        return findmin(root->left);
    }
}

Node *findmax(Node *root)
{
    if(root==NULL)
    {
        return NULL;
    }
    if(root->right ==NULL)
    {
        return root;
    }else
    {
        return findmax(root->right);
    }
}

Node *getrootnode(Node *n,char *str)  // get parent node
{
    while(n)
    {
        if(strcasecmp(str,n->key)==0)
        {
            return n;
        }else if(strcasecmp(str,n->key)<0)
        {
            if(strcasecmp(str,n->left->key)==0)
                return n;
            n=n->left;
        }else
        {
            if(strcasecmp(str,n->right->key)==0)
                return n;
            n=n->right;
        }
    }
    return NULL;
}

Node *findsucc(Node *root,Node *n)
{
    if(n==NULL)
    {
        return NULL;
    }
    Node *succ=NULL;
    if(n->right!=NULL)
    {
        return findmin(n->right);
    }else
    {
        Node *temp=getrootnode(root,n->key);
        while(1)
        {
            if(temp->left!=NULL){
            if(strcasecmp(temp->left->key,n->key)==0)
            {
                succ=temp;
                return succ;
            }else{
            n=temp;
            temp=getrootnode(root,temp->key);
            }
            }else{
            n=temp;
            temp=getrootnode(root,temp->key);
            }
        }
    }
}

Node *findpre(Node *root,Node *n)
{
    if(n==NULL)
    {
        return NULL;
    }
    Node *pre=NULL;
    if(n->left !=NULL)
    {
        return findmax(n->left);
    }else
    {
        Node *temp=getrootnode(root,n->key);
        while(1)
        {
            if(temp->right!=NULL){
            if(strcasecmp(temp->right->key,n->key)==0)
            {
                pre=temp;
                return pre;
            }else{
            n=temp;
            temp=getrootnode(root,temp->key);
            }
            }else{
            n=temp;
            temp=getrootnode(root,temp->key);
            }
        }
    }
}

int main()
{
    Node **n = load();
    Node *root = NULL;
    Node *succ=NULL;
    Node *pre=NULL;
    printf("Dictionary Loaded Successfully...!\n");
    printf("..................................\n");
    printf("Size = %d\n",i);
    printf("..................................\n");
    int k;
    for(k=0;k<i;k++)
    {
        root=insert(root,n[k]->key);
    }
    printf("Height = %d\n",height(root));
    printf("..................................\n");
    printf("Enter a sentence :\n");
    char word[50],*token;
    gets(word);
    token=strtok(word," ");
    while(token != NULL)
    {
        Node *k=search(root,token);
        if(k == NULL)
        {
            k=searchbeforenull(root,token);//get node where strcasecmp = 1 and n->right==NULL or strcasecmp=-1 and n->left==NULL
            printf("%s - Incorrect, Suggestions : ",token);
            printf("%s ",k->key);
            succ=findsucc(root,k);
            pre=findpre(root,k);
            printf("%s ",succ->key);
            printf("%s",pre->key);
        }else{
            printf("%s - CORRECT ",token);
        }
        printf("\n");
        token=strtok(NULL," ");
    }
    return 0;
}

