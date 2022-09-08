void cnfparser(header head)
{
    FILE *fp;
    printf("请输入所给算例文件！\n");
    scanf("%s",filename);
    if(!(fp=fopen(filename,"r"))) 
    {
        printf("所给算例无法正常读取！");
        exit(-1);
    }
    char a[1024],c;
    fscanf(fp,"%c",&c);
    while(c!='p')
    {
        fscanf(fp,"%[^\n]",a);
        fgetc(fp);
        fscanf(fp,"%c",&c);
    }//将所有注释略过
    int i;
    clausehead prehead;
    prehead=(clausehead)malloc(sizeof(clause));
    head->nexth=prehead;
    prehead->nextn=NULL;
    fscanf(fp,"%s%d%d",a,&head->numvar,&head->numclause);
    for(i=1;i<=head->numclause;i++)
    {
        int Var,len=0;
        fscanf(fp,"%d",&Var);
        clausehead phead;
        phead=(clausehead)malloc(sizeof(clause));
        clausenode pnode,prenode;
        prenode=(clausenode)malloc(sizeof(node));
        phead->nextn=prenode;
        while(Var)
        {
            pnode=(clausenode)malloc(sizeof(clausenode));
            pnode->Var=Var;
            prenode->next=pnode;
            prenode=pnode;
            len++;
            fscanf(fp,"%d",&Var);
        }
        prenode->next=NULL;
        phead->len=len;
        prehead->nexth=phead;
        prehead=phead;
        if(i==head->numclause) prehead->nexth=NULL;
    }
    fclose(fp);
}
