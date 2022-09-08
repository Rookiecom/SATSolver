int base[10][10]=
{   
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 9, 7, 8, 3, 1, 2, 6, 4, 5},
    {0, 3, 1, 2, 6, 4, 5, 9, 7, 8},
    {0, 6, 4, 5, 9, 7, 8, 3, 1, 2},
    {0, 7, 8, 9, 1, 2, 3, 4, 5, 6},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {0, 4, 5, 6, 7, 8, 9, 1, 2, 3},
    {0, 8, 9, 7, 2, 3, 1, 5, 6, 4},
    {0, 2, 3, 1, 5, 6, 4, 8, 9, 7},
    {0, 5, 6, 4, 8, 9, 7, 2, 3, 1},
};
int change[10][10]=
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
};
void creatematrix(int m[][10],int num)
{
    srand(time(NULL));
    int flag1[10]={0},a=1;
    while(a!=(num+1))
    {
        a+=3;
        for(int i=a-3;i<a;i++)
        {
            int temp;
            do{
                temp=rand()%(a-1)+1;
            }while(flag1[temp]!=0);
            flag1[temp]=1;
            m[i][temp]=1;
        }
    }
    int flag2[4]={0};
    for(int i=1;i<=num/3;i++)
    {
        int temp;
        do{
            temp=rand()%(num/3)+1;
        }while(flag2[temp]!=0);
        flag2[temp]=1;
        if(temp==i) continue;
        int j,k;
        for(j=1;j<=3;j++)
            for(k=1;k<=9;k++)
            {
                if(m[3*i+j][k] || m[3*temp+j][k])
                {
                    int t=m[3*i+j][k];
                    m[3*i+j][k]=m[3*temp+j][k];
                    m[3*temp+j][k]=t;
                }
            }     
    }
    for(int i=(num+1);i<=9;i++) m[i][i]=1;
    return ;
}
void create_single_sudoku(int l[][10],int r[][10],int s[][10])
{
    int i,j,k;
    for(i=1;i<=9;i++)
        for(j=1;j<=9;j++)
            for(k=1;k<=9;k++) s[i][j]+=l[i][k] * r[k][j];
    return ;
}
void dig(int sudoku1[][10],int sudoku2[][10])
{
    int flag1[10][10]={0},flag2[10][10]={0};
    srand(time(NULL));
    int x,y;
    for(int i=0;i<48;i++)
    {
        do{
            x=rand()%9+1;
            y=rand()%9+1;
        }while(flag1[x][y]==1 &&(x<7 || y<7));
        sudoku1[x][y]=0;
        flag1[x][y]=1;
    }
    for(int i=0;i<54;i++)
    {
        do{
            x=rand()%9+1;
            y=rand()%9+1;
        }while(flag2[x][y]==1);
        sudoku2[x][y]=0;
        flag2[x][y]=1;
    }
    return ;
}
void print(int sudoku1[][10],int sudoku2[][10])
{
    int i, j, ii, jj;
    printf("-------------------------------\n");
    for (i = 1; i <=6; i++) {
        for (j = 1; j <= 9; j++) {
            if (j % 3 == 1) printf("|");
            if (sudoku1[i][j] != 0) printf("%2d ", sudoku1[i][j]);
            else printf(" _ ");
            if (j == 9) printf("|\n");
        }
        if (i == 3) printf("-------------------------------\n");
        if (i == 6) printf("---------------------------------------------------\n");
    }
    for (i = 7; i <= 9; i++)
    {
        for(j = 1; j <= 9; j++)
        {
            if (j % 3 == 1) printf("|");
            if (sudoku1[i][j] != 0) printf("%2d ", sudoku1[i][j]);
            else printf(" _ ");
        }
        for(j = 4; j <= 9; j++)
        {
            if (j % 3 == 1) printf("|");
            if (sudoku2[i-6][j] != 0) printf("%2d ", sudoku2[i-6][j]);
            else printf(" _ ");
            if (j == 9) printf("|\n");
        }
    }
    printf("---------------------------------------------------\n");
    for(i = 4; i <= 9; i++)
    {
        printf("                    ");
        for(j = 1; j <= 9; j++)
        {
            if (j % 3 == 1) printf("|");
            if (sudoku2[i][j] != 0) printf("%2d ", sudoku2[i][j]);
            else printf(" _ ");
            if (j == 9) printf("|\n");
        }
        if (i == 6) {printf("                    ");printf("-------------------------------\n");}
        if (i == 9) {printf("                    ");printf("-------------------------------\n");}
    }
}
void translate_sudoku(char filename[],int sudoku1[][10],int sudoku2[][10])
{
    ofstream fp;
    fp.open(filename,ios::out);
    fp << "p cnf 2000 ";
    int temp_su = 24138;
    for(int i = 1; i <= 9; i++)
    {
        for(int j = 1; j <= 9; j++)
        {
            if(sudoku1[i][j] != 0)   temp_su++;
            if(sudoku2[i][j] != 0)  temp_su++;   
        }
    }
    fp << temp_su << endl;
    //输出单字句语句
    for(int i = 1; i <= 9; i++)
    {
        for(int j = 1; j <= 9; j++)
        {
            if(sudoku1[i][j] != 0)
                fp << i * 100 + j * 10 + sudoku1[i][j] << " " << 0 << endl;
            if(sudoku2[i][j] != 0)
                fp << 1000 + i * 100 + j * 10 + sudoku2[i][j] << " " << 0 << endl;
        }
    }
    //输出宫格部分
    for(int i = 1; i <= 9; i++)
    {
        for(int j = 1; j <= 9; j++)
        {
            for(int k = 1; k <= 9; k++) fp << i * 100 + j * 10 + k << " ";
            fp << 0 << endl;
            for(int k = 1; k <= 8; k++)
                for(int kk = k + 1; kk <= 9; kk++)
                    fp << -1 * (i * 100 + j * 10 + k) << " " << -(i * 100 + j * 10 + kk) << " " << 0 << endl;
            for(int k = 1; k <= 9; k++) fp << 1000 + i * 100 + j * 10 + k << " ";
            fp << 0 << endl;
            for(int k = 1; k <= 8; k++)
                for(int kk = k + 1; kk <= 9; kk++)
                    fp << -1 * (1000 + i * 100 + j * 10 + k) << " " << -(1000 + i * 100 + j * 10 + kk) << " " << 0 << endl;
        }
    }
    //输出行部分
    for(int i=1;i<=9;i++)  //第i行
    {
        for(int j=1;j<=9;j++) //第j个数字
        {
            for(int k=1;k<=9;k++)   //第几列
                fp << i*100+k*10+j << " ";
            fp << 0 <<endl;
        }
        for(int j=1;j<=9;j++)      //不同时为j
        {
            for(int k=1;k<=8;k++)
            {
                for(int kk=k+1;kk<=9;kk++)
                fp << -(i*100+k*10+j) << " " << -(i*100+kk*10+j) << " " << 0 << endl;
            }
        }
        for(int j=1;j<=9;j++) //数字
        {
            for(int k=1;k<=9;k++)   //列数
            {
                fp << 1000+i*100+k*10+j << " ";
            }
            fp << 0 <<endl;
        }
        for(int j=1;j<=9;j++)      //不同时为j
        {
            for(int k=1;k<=8;k++)
            {
                for(int kk=k+1;kk<=9;kk++)
                fp << -(1000+i*100+k*10+j) << " " << -(1000+i*100+kk*10+j) << " " << 0 << endl;
            }
        }
    }
    //输出列部分
    for(int i=1;i<=9;i++)  //第i列
    {
        for(int j=1;j<=9;j++) //数字
        {
            for(int k=1;k<=9;k++)   //行数
            {
                fp << k*100+i*10+j << " ";
            }
            fp << 0 <<endl;
        }
        for(int j=1;j<=9;j++)      //不同时为j
        {
            for(int k=1;k<=8;k++)
            {
                for(int kk=k+1;kk<=9;kk++)
                fp << -(k*100+i*10+j) << " " << -(kk*100+i*10+j) << " " << 0 << endl;
            }
        }
        for(int j=1;j<=9;j++) //数字
        {
            for(int k=1;k<=9;k++)   //行数
            {
                fp << 1000+k*100+i*10+j << " ";
            }
            fp << 0 <<endl;
        }
        for(int j=1;j<=9;j++)      //不同时为j
        {
            for(int k=1;k<=8;k++)
            {
                for(int kk=k+1;kk<=9;kk++)
                fp << -(1000+k*100+i*10+j) << " " << -(1000+kk*100+i*10+j) << " " << 0 << endl;
            }
        }
    }
    //单元格部分
    for(int i=1;i<=7;i+=3)         //第i个九宫格的第一行
    {
        for(int j=1;j<=7;j+=3)    //第j个九宫格的第一列
        {
            for(int k=1;k<=9;k++)  //k是数
            {
                for(int ii=i;ii<=i+2;ii++)
                {
                    for(int jj=j;jj<=j+2;jj++)
                    {
                        fp << ii*100+jj*10+k << " ";
                    }
                }
                fp << 0 << endl;
            }
            for(int n=1;n<=9;n++)       //数      
            {
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第一个的比较状况
                {
                    fp << -(i*100+j*10+n) << " " << -(i*100+j*10+n+100) << " " << 0 << endl;
                    fp << -(i*100+j*10+n) << " " << -(i*100+j*10+n+200) << " " << 0 << endl;
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -i << j << n << " " << -i << k << n << " " << 0 << endl;
                        fp << -i << j << n << " " << -(i+1) << k << n << " " << 0 << endl;
                        fp << -i << j << n << " " << -(i+2) << k << n << " " << 0 << endl;
                    }
                }
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第二个的比较状况
                {
                    fp << -(i+1) << j << n << " " << -(i+2) << j << n << " " << 0 << endl;
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -(i+1) << j << n << " " << -i << k << n << " " << 0 << endl;
                        fp << -(i+1) << j << n << " " << -(i+1) << k << n << " " << 0 << endl;
                        fp << -(i+1) << j << n << " " << -(i+2) << k << n << " " << 0 << endl;
                    }
                }
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第三个的比较状况
                {
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -(i+2) << j << n << " " << -i << k << n << " " << 0 << endl;
                        fp << -(i+2) << j << n << " " << -(i+1) << k << n << " " << 0 << endl;
                        fp << -(i+2) << j << n << " " << -(i+2) << k << n << " " << 0 << endl;
                    }
                }
            }
            for(int k=1;k<=9;k++)  //k是数
            {
                for(int ii=i;ii<=i+2;ii++)
                {
                    for(int jj=j;jj<=j+2;jj++)
                    {
                        fp << 1000+ii*100+jj*10+k << " ";
                    }
                }
                fp << 0 << endl;
            }
            for(int n=1;n<=9;n++)       //数      
            {
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第一个的比较状况
                {
                    fp << -1 << i << j << n << " " << -1 << i+1 << j << n << " " << 0 << endl;
                    fp << -1 << i << j << n << " " << -1 << i+2 << j << n << " " << 0 << endl;
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -1 << i << j << n << " " << -1 << i << k << n << " " << 0 << endl;
                        fp << -1 << i << j << n << " " << -1 << i+1 << k << n << " " << 0 << endl;
                        fp << -1 << i << j << n << " " << -1 << i+2 << k << n << " " << 0 << endl;
                    }
                }
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第二个的比较状况
                {
                    fp << -1 << i+1 << j << n << " " << -1 << i+2 << j << n << " " << 0 << endl;
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -1 << i+1 << j << n << " " << -1 << i << k << n << " " << 0 << endl;
                        fp << -1 << i+1 << j << n << " " << -1 << i+1 << k << n << " " << 0 << endl;
                        fp << -1 << i+1 << j << n << " " << -1 << i+2 << k << n << " " << 0 << endl;
                    }
                }
                for(int jj=j;jj<=j+2;jj++)    //去分析每一列的第三个的比较状况
                {
                    for(int k=jj+1;k<=j+2;k++)
                    {
                        fp << -1 << i+2 << j << n << " " << -1 << i << k << n << " " << 0 << endl;
                        fp << -1 << i+2 << j << n << " " << -1 << i+1 << k << n << " " << 0 << endl;
                        fp << -1 << i+2 << j << n << " " << -1 << i+2 << k << n << " " << 0 << endl;
                    }
                }
            }
        }
    }
    //关联部分
    for(int i=7;i<=9;i++)
    {
        for(int j=7;j<=9;j++)        
        {
            for(int k=1;k<=9;k++)
            {
                fp << -(i*100+j*10+k) << " " << 1000+(i-6)*100+(j-6)*10+k << " " << 0 << endl;
                fp << i*100+j*10+k << " " << -(1000+(i-6)*100+(j-6)*10+k) << " " << 0 << endl;
            }
        }
    }
    fp << 0 << endl;
    fp.close();
    return ;
}
void copy_sudoku(int sudoku1[][10],int sudoku2[][10],int sudoku3[][10],int sudoku4[][10])
{
    for(int i=1;i<=9;i++)
        for(int j=1;j<=9;j++) sudoku3[i][j]=sudoku1[i][j],sudoku4[i][j]=sudoku2[i][j];
    return ;
}
void create_two_sudoku(int sudoku1[][10],int sudoku2[][10],int sudoku3[][10],int sudoku4[][10])
{
    int rowmatrix1[10][10]={0},rowmatrix2[10][10]={0},colmatrix1[10][10]={0},colmatrix2[10][10]={0};
    int temp1[10][10]={0},temp2[10][10]={0},temp3[10][10]={0},temp[10][10]={0};
    creatematrix(rowmatrix1,9);
    creatematrix(colmatrix1,9);
    creatematrix(rowmatrix2,6);
    creatematrix(colmatrix2,6);

    create_single_sudoku(rowmatrix1,base,temp);
    create_single_sudoku(temp,colmatrix1,sudoku1);
    create_single_sudoku(rowmatrix2,sudoku1,temp1);
    create_single_sudoku(temp1,colmatrix2,temp2);
    create_single_sudoku(temp2,change,temp3);
    create_single_sudoku(change,temp3,sudoku2);
    printf("完整的双数独已生成\n如下：\n");
    copy_sudoku(sudoku1,sudoku2,sudoku3,sudoku4);
    print(sudoku1,sudoku2);
    printf("空缺的双数独已生成\n如下：\n");
    dig(sudoku1,sudoku2);
    print(sudoku1,sudoku2);
    char name[20]="sudoku.cnf";
    translate_sudoku(name,sudoku1,sudoku2);

    return ;
    
}
int sudoku_schedule()
{
    int op=1;
    int sudoku1[10][10]={0},sudoku2[10][10]={0},sudoku3[10][10]={0},sudoku4[10][10]={0};
    while (op) 
    {
		printf("\n\n");
		printf("Menu for soduku\n");
		printf("----------------------------- \n");
		printf("1. 数独生成        2. 数独试玩      \n");
        printf("3. 查看参考答案     0. 退出       \n");
		printf("请选择你的操作[0~4]:");
        scanf("%d",&op);
        switch (op)
        {
            case 0:
                {
                    printf("欢迎下次进入数独模块！\n");
                    return 0;
                }
            case 1:
            {
                create_two_sudoku(sudoku1,sudoku2,sudoku3,sudoku4);
                break;
            }      
            case 2:
            {
                printf("请输入1 2 2 3这样格式的数据\n其中1代表上半个数独，2代表下半个数独\n2 2代表所在数独的坐标！\n3代表欲填入的数字\n");
                printf("若输入错误，系统将会提示！\n若输入正确，系统将会输出填入后的图片！\n");
                int flag,row,col,number;
                scanf("%d%d%d%d",&flag,&row,&col,&number);
                if(flag==1)
                {
                    if((row>=1 && row<=9) && (col>=1 && col<=9))
                    {
                        if(sudoku3[row][col]==number)
                        {
                            printf("good job\n");
                            sudoku1[row][col]=number;
                            if((row>=7 && row<=9) && (col>=7 && col<=9))
                            {
                                sudoku2[row-6][col-6]=number;
                            }
                            print(sudoku1,sudoku2);
                        }
                        else printf("很遗憾，错误\n");
                    }
                    else printf("格式不对\n");
                }
                else if(flag==2)
                {
                    if((row>=1 && row<=9) && (col>=1 && col<=9))
                    {
                        if(sudoku4[row][col]==number)
                        {
                            printf("good job\n");
                            sudoku2[row][col]=number;
                            print(sudoku1,sudoku2);
                        }
                        else printf("很遗憾，错误\n");
                    }
                    else printf("格式不对\n");
                }
                else printf("格式不对\n");
                break;
            }
            case 3:
            {
                print(sudoku3,sudoku4);
                break;
            }
        }
    }
}