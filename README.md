# ping

## Notes

### Allowed Function

gettimeofday.
exit.
inet_ntop. / inet_pton.
signal / alarm.
socket / setsockopt.
sendto / recvmsg.
getpid / getuid.
getaddrinfo / freeaddrinfo.
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

### Output

each time a we received a response we print it;

