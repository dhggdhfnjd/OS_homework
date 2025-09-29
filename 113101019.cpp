#include<iostream>
#include<vector>
#include<cstdint>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>
using namespace std;
int main()
{
    int dim,count=0;
    cout<<"Input the matrix dimension:";
    cin>>dim;
    
    
    vector <vector<uint32_t> > A(dim,vector<uint32_t>()),B(dim,vector<uint32_t>());
    for(int i=0;i<dim;i++)
    {
        for(int j=0;j<dim;j++)
        {
            A[i].push_back(count);
            B[i].push_back(count);
            count++;
        }
    }
    // for(int i=0;i<dim;i++)
    // {
    //     for(int j=0;j<dim;j++)
    //     {
    //         printf("%d ",A[i][j]);
    //     }
    //     printf("\n");
    // }
    
    for(int j=1;j<=16;j++)
    {
        struct timeval start, end;
        int shmid = shmget(IPC_PRIVATE, dim*dim* sizeof(uint32_t), IPC_CREAT | 0600);
        uint32_t* C = (uint32_t*) shmat(shmid, NULL, 0);  
        gettimeofday(&start, NULL);
        int time=dim/j;
        for(int i=1;i<=j;i++)
        {
            pid_t child=fork();
            if(child==0)
            {
                uint32_t sum=0;
                if(i==j)
                {
                    for(int r=time*(i-1);r<dim;r++)
                    {
                        for(int s=0;s<dim;s++)
                        {
                            for(int k=0;k<dim;k++)
                            {
                                sum=sum+A[s][k]*B[k][r];
                            }
                            C[s*dim+r]=sum;
                            sum=0;
                        }
                    }
                }
                else
                for(int r=time*(i-1);r<time*i;r++)
                {
                    for(int s=0;s<dim;s++)
                    {
                        for(int k=0;k<dim;k++)
                        {
                            sum=sum+A[s][k]*B[k][r];
                        }
                        C[s*dim+r]=sum;
                        sum=0;
                    }
                }
                exit(0);
            }
        }
       
        for(int i=0;i<j;i++)
        wait(NULL); 
        gettimeofday(&end, NULL);
        double sec = (end.tv_sec-start.tv_sec)+(double)(end.tv_usec-start.tv_usec)/1000000;
        uint32_t checksum=0;
        for (int idx=0;idx<dim*dim;++idx) 
        checksum+=C[idx];
        cout<<"Multiplying matrices using "<<j<<" processes"<<endl;
        cout<<"Elapsed time="<<sec<<"sec,";
        cout<<"Checksum="<<checksum<<"\n";
        shmdt(C);
        shmctl(shmid, IPC_RMID, NULL);
    }
}