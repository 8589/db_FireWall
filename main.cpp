#include "firewall/firewall.h"


int get_a_son_process(int flag)
{
	printf("get a new son process\n");
	pid_t pid = fork();
	if(pid < 0)
	{
		cerr << "fork error";
	}
	else if(pid == 0)
	{
		if(flag == 0)
			printf("fiewall start\n");
		else
			printf("comm_with_web_UI start\n");
		sleep(20);
		exit(0);
	}
	else
	{
		return pid;
	}
	return 0;
}

int check_son_process_exist(pid_t pid)
{
	printf("check %d\n", pid);
	FILE* fp;
	char cmd[50];
	char buf[MSGSIZE];
	memset(cmd,0,sizeof(cmd));
	sprintf(cmd,"ps -ef|grep %d", pid);
	if( (fp=popen(cmd, "r")) == NULL )
	{
		printf("popen error\n");
		exit(2);
	}
	if(fgets(buf, MSGSIZE, fp) != NULL)
	{
		printf("result: %s\n", buf);
		return 1;
	}
	return 1;
}


int main()
{
	firewall fw;
	pid_t fw_pid = fork();
	if(fw_pid < 0)
	{
		cerr << "fork for firewall error!" << endl;
	}
	else if(fw_pid == 0)
	{
		//printf("fiewall start\n");
		//sleep(10);
		//exit(0);
		fw.start_firewall();
		
	}
	else
	{
		pid_t web_comm_pid = fork();
		if(web_comm_pid < 0)
		{
			cerr << "fork for web commuication error!" << endl;
		}
		else if(web_comm_pid == 0)
		{
			//printf("comm_with_web_UI start\n");
			//sleep(10);
			//exit(0);
			fw.comm_with_web_UI();
			
		}
		else
		{
			printf("%d %d\n", fw_pid, web_comm_pid);
			
			printf("main pro to listen...\n");
			while(1)
			{
				sleep(3600);
				/*
				printf("main....\n");
				if(!check_son_process_exist(fw_pid))
				{
					fw_pid = get_a_son_process(0);
				}
				if(!check_son_process_exist(web_comm_pid))
				{
					web_comm_pid = get_a_son_process(1);
				}
				sleep(15);
				*/
			}

		}
	}
	printf("end\n");
	return 0;
}


