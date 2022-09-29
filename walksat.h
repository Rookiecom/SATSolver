void randomvector(header head,int a[])
{
    srand(time(NULL));
    int temp;
    for(int i=1;i<=head->numvar;i++) 
    {
        temp=rand()%2;
        a[i]=temp;
    }
    return ;
}
bool check1(header head,int guess[])
{
    sat_information a[head->numvar+1]={0};
    clausehead phead=head->nexth->nexth;
    int flag=1;
    while(phead)
    {
        clausenode pnode=phead->nextn->next;
        int correctnum=0,tempvar;
        while(pnode)
        {
            if((pnode->Var>0 && guess[abs(pnode->Var)]==1) || (pnode->Var<0 && guess[abs(pnode->Var)]==0))
            {
                if(!correctnum) tempvar=pnode->Var;
                correctnum++;
            }
            else flag=0;
            pnode=pnode->next;
        }
        if(correctnum==0)
        {
            pnode=phead->nextn->next;
            while(pnode)
            {
                a[abs(pnode->Var)].usnum++;
                pnode=pnode->next;
            }
        }
        else if(correctnum==1) a[abs(tempvar)].snum++;
        phead=phead->nexth;
    }
    if(flag==1) return true;
    else   
    {
        int maxd=0,maxvar;
        for(int i=1;i<=head->numvar;i++)
        {
            a[i].d=a[i].usnum-a[i].snum;
            if(a[i].d>maxd)
            {
                maxd=a[i].d;
                maxvar=i;
            }
        }
        if(maxd>0) guess[maxvar]=!guess[maxvar];
        return false;
    }
}
bool check2(header head,int guess[],clausehead wrongclause[],int &num)
{
    clausehead phead=head->nexth->nexth;
    while(phead)
    {
        clausenode pnode=phead->nextn->next;
        int flag=0;
        while(pnode)
        {
            if((pnode->Var>0 && guess[abs(pnode->Var)]==1) || (pnode->Var<0 && guess[abs(pnode->Var)]==0))
            {
                flag=1;
                break;
            }
            pnode=pnode->next;
        }
        if(flag==0) wrongclause[num++]=phead;
        phead=phead->nexth;
    }
    if(num==0) return true;
    else
    {
        srand(time(NULL));
        int choose;
        do{
            choose=rand()%num;
            phead=wrongclause[choose];
            clausenode pnode=phead->nextn->next;
            while(pnode)
            {
                if((pnode->Var>0 && guess[(abs(pnode->Var))]==0) || pnode->Var<0 && guess[abs(pnode->Var)]==1)
                break;
                pnode=pnode->next;
            }
            if(pnode) 
            {
                choose=pnode->Var;
                break;
            }
        }while(1);
        if(choose>0) guess[choose]=1;
        else guess[-choose]=0;
    }
}
bool walksat_schedule(header head, int *s)
{
    int guess[head->numvar+1],pos,isright=1;
    clausehead wrongclause[head->numclause]={0};
    for(int i=0;i<N && isright;i++)
    {   
        srand(time(NULL));
        memset(guess,0,sizeof(guess));
        randomvector(head,guess);
        pos=1;
        for(int j=0;j<flip;j++)
        {
            if(j%100==0) srand(time(NULL));
            double x=rand()/double(RAND_MAX);
            //printf("%lf ",x);
            int temp=x>p?0:1;
            //printf("%d \n",temp);
            int num=0;
            if(temp==0) 
            {
                if(check1(head,guess)==true)
                {
                    isright=0;
                    break;
                }   
            }
            else
            {
                if(check2(head,guess,wrongclause,num)==true)
                {
                    isright=0;
                    break;
                }
            } 
        }
    }
    for(int i=1;i<=head->numvar;i++) s[i]=guess[i];
    if(isright==0) return true;
    else return false;
}