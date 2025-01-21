#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    int server_sock, server_size;
    
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    int socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_desc < 0){
        printf("\nError, Socket not connected\n");
        exit(0);
        
    }
    printf("\nSocket connected succesfully\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    Frame frame_send;
    Frame frame_recv;
    int frame_id=0;
    int ack_recv = 1;
    char buffer[1024];
    
    
    
    while(1){ //Stop-N-Wait
        if(ack_recv == 1){
            frame_send.seq = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;
            
            printf("\nEnter Data\n");
            scanf("%s",buffer);
            strcpy(frame_send.packet, buffer);
            sendto(socket_desc, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&server_addr, server_struct_length);
            printf("\nFrame Send");
        }
        
        int addr_size = sizeof(server_addr);
        int frame_recv_size = recvfrom(socket_desc, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&server_addr,
                        &server_struct_length);
                        
        if(frame_recv_size > 0 && frame_recv.seq == 0, frame_recv.ack == frame_id + 1){
            printf("\nAcknowledgement recieved");
            ack_recv = 1;
        } else {
            printf("\nAcknowledgemnt not recieved");
            ack_recv = 0;
        }
        
        frame_id++;
    
    }
    
    
    close(socket_desc);
    
    return 0;
    
}
