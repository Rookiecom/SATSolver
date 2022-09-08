#include<iostream>
#include<cstdlib>
#include<math.h>
#include<time.h>
#include<cstring>
#include<fstream>
#include<bits/stdc++.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1

#define N 3
#define flip 50
const float p=0.5;

typedef struct node
{
    int Var;
    node *next;
}node,*clausenode;
typedef struct clause
{
    clause *nexth;
    int len;
    node *nextn;
}clause ,*clausehead;
typedef struct tablehead
{
    int numvar;
    int numclause;
    clause *nexth;
}tablehead,*header;
typedef struct linearlist
{
    int len;
    int *array;
}linearlist;
typedef struct sat_information
{
    int snum;
    int usnum;
    int d;
}sat_information;
typedef struct data
{
    int p;
    int n;
    int s;
    int var;
}data;

char filename[100];
linearlist ans;
linearlist walksat;
int ordernum;

int mainsolve1(header head);
int mainsolve2(header head,int assumvar,linearlist suspect);
int mainsolve3(header head,int assumvar,linearlist suspect);