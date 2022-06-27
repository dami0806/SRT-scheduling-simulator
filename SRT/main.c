#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <memory.h>
#include <math.h>


#define LEVEL 5
#define Queue_Max 100

typedef struct process // 프로세스 구조체
{
	int pid;
	int arr_t;
	int serv_t;
	int start_t;
	int end_t;
	int banhwan;
	int remain_time; // 남은 시간. 추가
	int waiting_time; // 대기 시간. 추가
	float average_banhwan;

}Process;

// 도착시간 순으로 정렬하는 함수
void p_sort(Process* p, int n) {
	Process tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (p[j].arr_t > p[j + 1].arr_t) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
			else if (p[j].arr_t == p[j + 1].arr_t && p[j].pid > p[j + 1].pid) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
		}
	}
}

// id순으로 정렬하는 함수
void p_resort(Process* p, int n) {
	Process tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (p[j].pid > p[j + 1].pid) {
				tmp = p[j + 1];
				p[j + 1] = p[j];
				p[j] = tmp;
			}
		}
	}
	for (int i = 0; i < n; i++) {
		printf("PID: %d\t도착시간: %d\t서비스시간: %d\t종료시간: %d\t반환시간: %d\t정규화된 반환시간: %.5f\n\n"
			, p[i].pid, p[i].arr_t, p[i].serv_t, p[i].end_t, p[i].banhwan, p[i].average_banhwan);
	}
}

// SRT 알고리즘
void p_srt(Process* p, int n) {
	// 남은 프로세스 수, 최소 남은시간(처음을 제외한), 반복문 i
	// now는 시스템 돌아가는 시간
	int remain, min, now_p, i;
	int now = 0;

	now_p = 0;
	remain = n;
	now = p[0].arr_t;

	// 전에 주신 파일에서는 문제가 없었지만, 문제가 발생하여
	// 아래 for문 추가. 서비스시간 이용해서 남은시간 값 넣어준것.
	for (int i = 0; i < n; i++) {
		p[i].remain_time = p[i].serv_t;
	}

	while (remain > 0) {
		min = 200;
		// 남은 동작 시간 순으로 실행하는 부분
		for (i = 0; p[i].arr_t <= now && i < n; i++) {
			if (p[i].remain_time < min && p[i].remain_time > 0) {
				now_p = i;
				min = p[i].remain_time;
			}
		}
		now++;
		p[now_p].remain_time--;

		if (p[now_p].remain_time == 0) {
			p[now_p].waiting_time = now - p[now_p].arr_t - p[now_p].serv_t;
			p[now_p].end_t = now;
			p[now_p].banhwan = p[now_p].end_t - p[now_p].arr_t;
			p[now_p].average_banhwan = ((float)p[now_p].waiting_time + (float)p[now_p].serv_t) / (float)p[now_p].serv_t;

			//printf("%d번째 프로세스가 종료되었습니다.\n", n - remain + 1);
			printf("PID: %d\t도착시간: %d\t서비스시간: %d\t종료시간: %d\t반환시간: %d\t정규화된 반환시간: %.5f\n\n"
				, p[now_p].pid, p[now_p].arr_t, p[now_p].serv_t, p[now_p].end_t, p[now_p].banhwan, p[now_p].average_banhwan);

			remain--;
		}
	}
}

// SRT알고리즘에 필요한 함수 정리
void SRT(Process* p, int n) {
	printf("\n\n\t\t<<SRT 알고리즘 >>\n\n\n");
	p_sort(p, n);
	p_srt(p, n);
	printf("\n\n\t\t<<SRT 알고리즘 결과(id순으로 정렬)>>\n\n\n");
	p_resort(p, n);
}

// input 텍스트 파일 읽어오기
int DataSet(Process queue[][Queue_Max], Process* processes, int* numOfData)
{


	char line[32] = { 0, };
	FILE* file = fopen("input.txt", "r");
	if (file == NULL)
		return -1;

	int _pid = -1, _arr_t = -1, _serv_t = -1; //여기에 담아서 pid arrv_t serv_t에 넣기
	char tmp = '\0'; //  콤마 받기 위한 변수

	int idxOfProcesses = 0;

	//input.txt에서 데이터를 읽어와 Process배열에 데이터를 순차적으로 저장
	while (1) {
		fscanf(file, "%d %c %d %c %d", &_pid, &tmp, &_arr_t, &tmp, &_serv_t);
		if (feof(file) || _pid == 0) //0일떄 종료
			break;

		processes[idxOfProcesses].pid = _pid;
		processes[idxOfProcesses].arr_t = _arr_t;
		processes[idxOfProcesses].serv_t = _serv_t;

		idxOfProcesses++;
	}
	*numOfData = idxOfProcesses; // 읽어온 프로세스의 갯수 저장
	//뒤에서 갯수를 다 사용하면 종료할것


	int idxOfQueue = 0;
	//순차적으로 저장한 Process배열내에 데이터 중 도착시간이 0 인 프로세스들은 Queue 0 단계에 집어넣는다.
	for (int i = 0; i < idxOfProcesses; ++i)
	{
		if (processes[i].arr_t == 0)
			queue[0][idxOfQueue++] = processes[i];
	}

	fclose(file);
	return idxOfQueue; // 큐에 들어온 프로세스 갯수 반환
}


// 메인함수
int main(void)
{
	Process SRT_p[Queue_Max] = { 0, };
	Process SRT_q[LEVEL][Queue_Max] = { 0, };

	int SRT_c = -1;

	DataSet(SRT_q, SRT_p, &SRT_c);
	SRT(SRT_p, SRT_c); // SRT 알고리즘 시작

	return 0;
}