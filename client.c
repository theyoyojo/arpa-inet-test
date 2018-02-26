#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 256

void error ( const char * msg )
{
  perror ( msg ) ;
  exit ( 0 ) ;
}

int main ( int argc , char * argv [] )
{

  int fd_socket , valid_addr , connect_success , send_int_success , get_reply_success ;
  struct sockaddr_in my_sock ;
  char buff [ BUFFER_SIZE ] ;

  memset ( buff , 0 , BUFFER_SIZE ) ;
  
  if ( argc < 3 )
  {
    fprintf ( stderr , "ERROR, you must enter a port number and address as an argument\n" ) ;
    exit ( 1 ) ;
  }

  fd_socket = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ; 
  if ( fd_socket < 0 ) error ( "ERROR, cannot open socket" ) ;
  
  memset ( & my_sock , 0 , sizeof ( my_sock ) ) ;

  my_sock .sin_family = AF_INET ;
  my_sock .sin_port = htons ( atoi ( argv [ 2 ] ) ) ;

  valid_addr = inet_aton ( argv [ 1 ] , & my_sock.sin_addr ) ;
  if ( valid_addr == -1 ) error ( "ERROR, invalid server address");
  
  int data , noc ;
  char c ;
  printf ( "Enter an integer for the server to examine:" ) ;
  while ( ( noc = scanf ( "%d" , & data ) != 1 ) )
  {
    printf ( "Invalid input. Try again:" ) ;
    while ( scanf ( "%c" , & c ) == 1 && c != '\n' ) ;
  }
  while ( scanf ( "%c" , & c ) == 1 && c != '\n' ) ;


  connect_success = connect ( fd_socket , & my_sock , sizeof ( my_sock ) ) ;
  if ( connect_success != 0 ) error ( "ERROR, cannot establish connection to server" ) ;

  send_int_success = write ( fd_socket , & data , sizeof ( data ) ) ; 
  //if ( send_int_success != 0 ) error ( "ERROR, cannot send data to server" ) ;

  get_reply_success = read ( fd_socket , & data , sizeof ( data ) ) ;
  //if ( get_reply_success != 0 ) error ( "ERROR, cannot getr response from server" ) ;

  printf ( "Got reply from server: %d\n" , data ) ;

  close ( fd_socket ) ;

  return 0 ;
}
