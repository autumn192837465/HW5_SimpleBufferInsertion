#include<msgpack.hpp>
#include<msgpack/fbuffer.hpp>
#include<unistd.h>
#include<stdlib.h>
#include<vector>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

struct NODE{
    double m_r;
    double m_c;
};
struct BUFFERS{
    int m_id;
    double m_r;
    double m_c;
    double m_cost;
};

int g_trash;
int ans[20];
int solution[20];

bool compare(BUFFERS a,BUFFERS b){
    return a.m_cost<b.m_cost;
}

double g_maxDelay;
int g_nodeNum;
int g_bufferNum;
//vector <BUFFERS>v_buffers;

double minCost;
double minDelay;

bool have_ans;

NODE node[20];
BUFFERS v_buffers[6];


void backtracking(int n,double lastDelay,double lastCost,double lastDscap){

    if(n==0){
        if(lastDelay<=g_maxDelay){
            if(lastCost<minCost){
                minCost=lastCost;
                minDelay=lastDelay;
                copy(ans,ans+g_nodeNum,solution);
                have_ans=1;
            }
            else if(lastCost==minCost && lastDelay<minDelay){
                minDelay=lastDelay;
                copy(ans,ans+g_nodeNum,solution);
                have_ans=1;
            }
        }
        return;
    }
    double currentDelay=0;
    double currentDscap=0;
    //no insert

    double noBufferDelay=0;
    double noBufferDscap=0;

    noBufferDscap=lastDscap+node[n-1].m_c;

    noBufferDelay=node[n-1].m_r*noBufferDscap +lastDelay ;
    if(noBufferDelay<=g_maxDelay){
        ans[n-1]=0;
        backtracking(n-1,noBufferDelay,lastCost,noBufferDscap);
    }



    //insert buffer


    for(int i=0;i<g_bufferNum;i++){
        currentDscap=lastDscap+node[n-1].m_c;
        currentDelay=node[n-1].m_r * v_buffers[i].m_c +v_buffers[i].m_r * currentDscap+lastDelay;

        if(currentDelay<=g_maxDelay && lastCost+v_buffers[i].m_cost<=minCost && (noBufferDelay>=currentDelay || noBufferDscap >=v_buffers[i].m_c)){
            ans[n-1]=i+1;
            backtracking(n-1,currentDelay,lastCost+v_buffers[i].m_cost,v_buffers[i].m_c);
        }
    }

}


int main()
{

	fstream fin,fout;
	fin.open("input.txt",ios::in);
	fout.open("output.txt",ios::out);

    int task;
    fin>>task;

    while(task--){

        fin>>g_maxDelay;
        fin>>g_nodeNum;

        for(int i=0;i<g_nodeNum;i++){
            fin>>g_trash;
            fin>>node[i].m_r;
            fin>>node[i].m_c;
        }



        fin>>g_bufferNum;


        BUFFERS tempBuffers;


        for(int i=0;i<g_bufferNum;i++){
            fin>>g_trash;
            fin>>v_buffers[i].m_r;
            fin>>v_buffers[i].m_c;
            fin>>v_buffers[i].m_cost;
        }


        //sort(v_buffers.begin(),v_buffers.end(),compare);

        //memset(ans,0,sizeof(ans));
        //memset(solution,0,sizeof(solution));
        minCost=1e9;
        minDelay=1e9;
        have_ans=0;

        backtracking(g_nodeNum,0,0,0);



        if(!have_ans)
            fout<<"NO SOLUTION";
        else{
            bool used=0;
            for(int i=0;i<g_nodeNum;i++){
                if(solution[i] && !used){
                    fout<<i+1<<" "<<solution[i]; //v_buffers[solution[i]-1].m_id;
                    used=1;
                }
                else if(solution[i] && used)
                    fout<<" "<<i+1<<" "<<solution[i];//v_buffers[solution[i]-1].m_id;
            }
        }
        fout<<endl;

        //v_buffers.clear();
    }


        fin.close();
        fout.close();
        return 0;
}
