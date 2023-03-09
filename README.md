# ping

## list

- [x] Parsing
- [ ] gestion erreur
- [x] determiner a qui ont veux envoyer la request | DNS getaddrinfo
- [x] create socket
- [ ] ICMP echo request creation
- [ ] send
- [ ] package received
- [ ] fonctionnement


## Notes

### Allowed Function

gettimeofday.

exit.

inet_ntop. / inet_pton.

signal / alarm.

socket create a socket file descriptor

setsockopt : modify the socket option with a int socket fd
setsockopt permet de set les options d'entête IP sur des socket raw

sendto / recvmsg.

getpid / getuid.


getaddrinfo / freeaddrinfo.

- transorm an error code (int) in a string for Human
strerror / gai_strerror.

printf and its family.

Your libft functions

### Basic

we send a packet and expecte a return


### DNS
Domain name system

### Input

- a Host
	- check host validity
	0.0.0.0
	255.255.255.255
	if not in this range : unknown host
	
	can be a fqdn
	google.com
	parsing specifique ?
	or use of a command ?
	
	mask reseau : describe which part is set for the network and which is for the host number

-- Possible option/s :
	-v
	-h
	-vh


#### Fonctionnement

we send a ICMP of type 8
echo -> cho -> ho -> o -> o -> o
and we should received a type 0 and echo reply

### Output

each time a we received a response we print it;


##### ERROR

we should read the man page of raw.

getaddrinfo: Servname not supported for ai_socktype
getaddrinfo take first the internet host and second the service

chequsum ?
