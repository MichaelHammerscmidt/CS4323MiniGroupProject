#ifndef IPC_DOT_H
#define IPC_DOT_H

void createProcesses(int nProcess, char *filename, char *column, char uniques[][15]);
void sendData(int pid, char *uniqueValue);
void receiveData(int pid);

#endif