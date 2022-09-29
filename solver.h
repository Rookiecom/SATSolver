int selectvar1(header head,linearlist suspect)
//选取策略，返回选取的变量
{
    int a[2*head->numvar+1],maxnum=0,maxvar;
    memset(a,0,sizeof(a));
    clausehead phead=head->nexth->nexth;
    while(phead)
    {
        if(phead->len==2)
        {
            clausenode pnode=phead->nextn->next;
            int t=abs(pnode->Var);
            if(pnode->Var>0) a[2*t-1]++;
            else a[2*t]++;
            t=abs(pnode->next->Var);
            if(pnode->next->Var>0) a[2*t-1]++;
            else a[2*t]++;
        }
        phead=phead->nexth;
    }
    for(int i=1;i<=2*head->numvar;i++)
    {
        if(a[i]>maxnum)
        {
            maxnum=a[i];
            if(i%2) maxvar=(i+1)/2;
            else maxvar=-i/2;
        }
    }
    if(maxnum>0) return maxvar;
    else return head->nexth->nexth->nextn->next->Var;
}
int selectvar2(header head,linearlist suspect)
//选取策略，返回选取的变量
{
    int a[head->numvar+1],maxnum=0,maxvar;
    memset(a,0,sizeof(a));
    clausehead phead=head->nexth->nexth;
    while(phead)
    {
        if(phead->len==2)
        {
            clausenode pnode=phead->nextn->next;
            int t=abs(pnode->Var);
            a[t]++;
            t=abs(pnode->next->Var);
            a[t]++;
        }
        phead=phead->nexth;
    }
    for(int i=1;i<=head->numvar;i++)
    {
        if(a[i]>maxnum)
        {
            maxnum=a[i];
            maxvar=a[i];
        }
    }
    if(maxnum>0)
    {
        if(walksat.array[maxvar]==0) return -maxvar;
        else return maxvar;
    }
    else return head->nexth->nexth->nextn->next->Var;
}
bool cmp(data a,data b)
{
    return  a.s<b.s;
}
int selectvar3(header head,linearlist suspect)
{
    for(int i=1;i<=head->numvar;i++)
        if(ans.array[i]==-1 && suspect.array[i]==-1) return -i;
}
int (*a[3])(header,linearlist)={selectvar1,selectvar2,selectvar3};
void changesuspect(int assumvar,linearlist &suspect)
{
    if(assumvar>0) suspect.array[assumvar]=1;
    else suspect.array[-assumvar]=0;
    return ;
}
void delsinglevar(header head,int *Var,int num)
//确定变元之后删除相关变元和子句
{
    clausehead prehead,phead;
    prehead=head->nexth;
    phead=prehead->nexth;
    while(phead)
    {
        clausenode prenode,pnode;
        int isright=0;
        prenode=phead->nextn;
        pnode=prenode->next;
        while(pnode && isright!=2)
        {
            int i;
            isright=0;
            for(i=0;i<num;i++)
            {
                if(pnode->Var==Var[i])
                {
                    isright=2;
                    break;
                }
                else if(pnode->Var==-Var[i])
                {
                    prenode->next=pnode->next;
                    free(pnode);
                    phead->len--;
                    isright=1;
                    break;
                }
            }
            if(isright==1) pnode=prenode->next;
            else if(isright==0) 
            {
                prenode=pnode;
                pnode=prenode->next;
            }            
        }
        if(isright==2)
        {
            prehead->nexth=phead->nexth;
            clausenode pre,p;
            pre=phead->nextn;
            p=pre->next;
            while(p)
            {
                free(pre);
                pre=p;
                p=p->next;
            }
            free(pre);
            free(phead);
            head->numclause--;
            phead=prehead->nexth;
        }
        else
        {
            prehead=phead;
            phead=prehead->nexth;
        }
    }
    return ;
}
int delsingle(header head,linearlist &suspect)
//将单子句的变元收集起来，并删除单子句；同时查看是否有空子句
//TRUE表示这次扫描有单子句，FALSE表示无，INFEASIBLE表示现在的数据无解
{
    int i,*V,num=0,Var;
    V=(int *)malloc((2*(head->numclause)) * sizeof(int));//V数组是记录需要删除的变元的
    clausehead prehead=head->nexth,phead=head->nexth->nexth;
    while(phead)
    {
        if(phead->len==0) return INFEASIBLE;
        if(phead->len==1)
        {
            clausenode prenode=phead->nextn;
            clausenode pnode=prenode->next;
            Var=pnode->Var;
            if(suspect.array[abs(Var)]==-1)
            {
                V[num++]=Var;
                if(Var>0) suspect.array[Var]=1;
                else suspect.array[-Var]=0;
                ans.len++;
            }
            else if((pnode->Var>0 && suspect.array[abs(Var)]==0) || (pnode->Var<0 && suspect.array[abs(Var)]==1)) return INFEASIBLE;
            head->numclause--;
            prehead->nexth=phead->nexth;
            free(phead);
            free(prenode);
            free(pnode);
            phead=prehead->nexth;
        }
        else
        {
            prehead=phead;
            phead=prehead->nexth;
        } 
    }
    if(num!=0) 
    {
        delsinglevar(head,V,num);
        return TRUE;
    }
    return FALSE;
}
void copy(header copyhead,header head)
{
    copyhead->numclause=head->numclause;
    copyhead->numvar=head->numvar;
    clausehead phead,copyprehead;
    copyprehead=(clausehead)malloc(sizeof(clause));
    copyhead->nexth=copyprehead;
    phead=head->nexth->nexth;
    while(phead)
    {
        clausehead copyphead=(clausehead)malloc(sizeof(clause));
        copyphead->len=phead->len;
        clausenode copyprenode,pnode,copypnode;
        copyprenode=(clausenode)malloc(sizeof(node));
        copyphead->nextn=copyprenode;
        copyprenode->next=NULL;
        pnode=phead->nextn->next;
        while(pnode)
        {
            copypnode=(clausenode)malloc(sizeof(node));
            copypnode->Var=pnode->Var;
            copypnode->next=copyprenode->next;
            copyprenode->next=copypnode;
            pnode=pnode->next;
        }
        copyprehead->nexth=copyphead;
        copyprehead=copyprehead->nexth;
        phead=phead->nexth;
    }
    copyprehead->nexth=NULL;
    return ;
}
void freeall(header head)
{
    clausehead prehead,phead;
    prehead=head->nexth->nexth;
    phead=prehead->nexth;
    free(head->nexth);
    head->nexth=NULL;
    free(head);
    head=NULL;
    while(phead)
    {
       
        if(prehead->len>0)
        { 
            clausenode prenode,pnode;
            prenode=prehead->nextn->next;
            pnode=prenode->next;
            free(prehead->nextn);
            prehead->nextn=NULL;
            while(pnode)
            {
                free(prenode);
                prenode=pnode;
                pnode=pnode->next;
            }
            free(prenode);
            prenode=NULL; 
            free(prehead);       
            prehead=phead;
            phead=phead->nexth;
        }
        else
        {
            free(prehead->nextn);
            prehead->nextn=NULL;
            free(prehead);
            prehead=phead;
            phead=phead->nexth;
        }

    }
    clausenode prenode,pnode;
    prenode=prehead->nextn;
    pnode=prenode->next;
    while(pnode)
    {
        free(prenode);
        prenode=pnode;
        pnode=pnode->next;
    }
    free(prenode);
    prenode=NULL;
    free(prehead);
    prehead=NULL;
    return ;
}
int mainsolve3(header head,int assumvar,linearlist suspect)
{
    linearlist copysuspect;
    copysuspect.len=suspect.len;
    copysuspect.array=(int *)malloc(sizeof(int) * (head->numvar+1));
    for(int i=1;i<=head->numvar;i++) copysuspect.array[i]=suspect.array[i];
    changesuspect(assumvar,copysuspect);
    copysuspect.len++;

    header copyhead;
    copyhead=(header)malloc(sizeof(tablehead));
    copy(copyhead,head);
    delsinglevar(copyhead,&assumvar,1);
    int temp;
    while((temp=delsingle(copyhead,copysuspect))==TRUE);
    if(temp==FALSE)
    {
        if(copyhead->numclause==0)
        {
            for(int i=1;i<=head->numvar;i++)
            {
                if(ans.array[i]==-1)
                {
                    if(copysuspect.array[i]==-1) ans.array[i]=0;
                    else ans.array[i]=copysuspect.array[i];
                } 
            }
            return TRUE;
        } 
        int proassumvar=a[ordernum-1](copyhead,copysuspect);
        if((mainsolve2(copyhead,proassumvar,copysuspect))==TRUE) return TRUE;
    }
    freeall(copyhead);
    return INFEASIBLE;
}
int mainsolve2(header head,int assumvar,linearlist suspect)
{
    linearlist copysuspect;
    copysuspect.len=suspect.len;
    copysuspect.array=(int *)malloc(sizeof(int) * (head->numvar+1));
    for(int i=1;i<=head->numvar;i++) copysuspect.array[i]=suspect.array[i];
    changesuspect(assumvar,copysuspect);
    copysuspect.len++;
    header copyhead;
    copyhead=(header)malloc(sizeof(tablehead));
    copy(copyhead,head);
    delsinglevar(copyhead,&assumvar,1);
    int temp;
    while((temp=delsingle(copyhead,copysuspect))==TRUE);
    if(temp==FALSE)
    {
        if(copyhead->numclause==0)
        {
            for(int i=1;i<=head->numvar;i++)
            {
                if(ans.array[i]==-1)
                {
                    if(copysuspect.array[i]==-1) ans.array[i]=0;
                    else ans.array[i]=copysuspect.array[i];
                } 
            }
            return TRUE;
        }
        int proassumvar=a[ordernum-1](copyhead,copysuspect);
        if((mainsolve2(copyhead,proassumvar,copysuspect))==TRUE) return TRUE;
    }
    freeall(copyhead);
    copyhead=(header)malloc(sizeof(tablehead));
    copy(copyhead,head);
    assumvar=-assumvar;
    if((mainsolve3(copyhead,assumvar,suspect))==TRUE) return TRUE;
    else 
    {
        free(copysuspect.array);
        return INFEASIBLE;
    }

}
int mainsolve1(header head)
{
    linearlist suspect;
    suspect.array=(int *)malloc(sizeof(int) * (head->numvar+1));
    memset(suspect.array,-1,sizeof(int) * (head->numvar+1));
    int assumvar=a[ordernum-1](head,suspect);
    changesuspect(assumvar,suspect);
    suspect.len++;
    header copyhead=(header)malloc(sizeof(tablehead));
    copy(copyhead,head);
    delsinglevar(copyhead,&assumvar,1);
    int temp;
    while((temp=delsingle(copyhead,suspect))==TRUE);
    if(temp==FALSE)
    {
        if(copyhead->numclause==0)
        {
            for(int i=1;i<=head->numvar;i++)
            {
                if(ans.array[i]==-1)
                {
                    if(suspect.array[i]==-1) ans.array[i]=0;
                    else ans.array[i]=suspect.array[i];
                } 
            }
            return TRUE;
        }
        int proassumvar=a[ordernum-1](copyhead,suspect);
        if((mainsolve2(copyhead,proassumvar,suspect))==TRUE) return TRUE;
    }
    free(suspect.array);
    assumvar=-assumvar;
    if(assumvar>0) ans.array[assumvar]=1;
    else ans.array[-assumvar]=0;
    ans.len++;
    delsinglevar(head,&assumvar,1);
    while((temp=delsingle(head,ans))==TRUE);
    if(temp==FALSE) return mainsolve1(head);
    else return INFEASIBLE;
}
int schedule(header head)
{
    clock_t start,end;
    double duration;
    start=clock();
    int len=strlen(filename);
    filename[len-3]='r',filename[len-2]='e',filename[len-1]='s';
    FILE *fp;
    fp=fopen(filename,"w+");
    int temp;
    while((temp=delsingle(head,ans))==TRUE);
    if(temp==INFEASIBLE) return INFEASIBLE;
    if((mainsolve1(head))==TRUE) 
    {
        end=clock();
        duration=(double )(1000 *(end-start))/CLOCKS_PER_SEC;
        fprintf(fp,"%c %d\n",'s',1);
        fprintf(fp,"%c ",'v');
        for(int i=1;i<=head->numvar;i++) 
        {
            if(ans.array[i]==1) fprintf(fp,"%d ",i);
            else fprintf(fp,"%d ",-i);
        }
        fprintf(fp,"\n%c %lf",'t',duration);
        fclose(fp);
        return TRUE;
    }
    else
    {
        end=clock();
        duration=(double )(1000 * (end-start))/CLOCKS_PER_SEC;
        fprintf(fp,"%c %d\n",'s',0);
        fprintf(fp,"%c %lf",'t',duration);
        fclose(fp);
        return INFEASIBLE;
    }
}