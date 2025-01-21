#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
typedef struct frame{
    int frame_kind;
    int ack;
    int seq;
    char packet[1024];
}Frame;
 
int main(){
    struct sockaddr_in client_addr, server_addr;
    int client_sock, client_size;
    
    char server_msg[2000], client_msg[2000];
    
    memset(server_msg, '\0', sizeof(server_msg));
    memset(client_msg, '\0', sizeof(client_msg));   
    int client_struct_length = sizeof(client_addr); 
    
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0) {
        printf("\nError , Socket creation failed!\n");
        exit(0);
    }
    printf("\nSocket Created Successfully\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0) {
        printf("\nBind Failed\n");
        exit(0);
    }
    printf("\nBind Success\n");
    
    Frame frame_send;
    Frame frame_recv;
    int frame_id = 0;
    
    
    while(1){ //Stop-N-Wait
        int frame_recv_size = recvfrom(socket_desc, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&client_addr, 
                        &client_struct_length);
        if(frame_recv_size >0 && frame_recv.frame_kind == 1 && frame_recv.seq == frame_id){
            printf("\nFrame recieved: %s",frame_recv.packet);
            frame_send.seq = 0;
            frame_send.ack = frame_recv.seq + 1;
            frame_send.frame_kind = 0;
            sendto(socket_desc, &frame_send, sizeof(frame_send), 0, (struct sockaddr*)&client_addr,
                client_struct_length);
            printf("\nAcknowlegement Send");
            
        } else {
            printf("\nFrame not recieved");
        }
        frame_id++;
    }
    
    
    close(socket_desc);
    
    return 0;
    
}
 
 
