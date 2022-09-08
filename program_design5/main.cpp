using namespace std;
#include"define.h"
#include"cnfparser.h"
#include"solver.h"
#include"display.h"
#include"sudoku.h"
#include"walksat.h"
int main()
{
    int op=1;
    while (op) 
    {
		printf("\n\n");
		printf("Menu for Program Design\n");
		printf("--------------------------------- \n");
		printf("1. SAT       2.数独      \n");
        printf("0. EXIT            \n");
		printf("请选择你的操作[0~2]:");
        scanf("%d",&op);
        switch (op)
        {
            case 0:
                {
                    printf("欢迎下次再使用本系统！\n");
			        return 0; 
                }
            case 1:
                {
                    printf("请输入欲选取的策略\n");
                    scanf("%d",&ordernum);
                    header head=(header)malloc(sizeof(tablehead));
                    cnfparser(head);
                    ans.array=(int *)malloc(sizeof(int) * (head->numvar+1));
                    walksat.array=(int *)malloc(sizeof(int) * (head->numvar+1));
                    walksat.len=0;
                    ans.len=0;
                    for(int i=1;i<=head->numvar;i++) ans.array[i]|=-1,walksat.array[i]|=-1;
                    walksat_schedule(head,walksat.array);
                    int temp=schedule(head);
                    if(temp==TRUE) printf("此文件所指向的sat问题可满足！\n并且找到一组可满足的解存放在%s\n",filename);
                    else printf("此文件所指向的sat问题不可满足！\n");
                    break; 
                }                
            case 2:                
                {
                    sudoku_schedule();
                    break; 
                }
                
        }
    }    
    return 0;
}
