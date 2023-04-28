#!/usr/bin/env bash

am_i_sudo(){

    if sudo -v;
    then
        printf "\n---< You have admin right. >---\n\n"
        return $(true)
    else
        printf "\n---> You do not have admin right. <---\n\n"
        return $(false)
    fi
}

am_i_sudo

if [[ $? == 0 ]]
then
    case $1 in

        icmp_input)
            # ICMP (Ping)
            sudo iptables -t filter -A INPUT -p icmp -j DROP
            printf "Block ICMP INPUT\n"
            ;;

        icmp)
            # ICMP (Ping)
            sudo iptables -t filter -A INPUT -p icmp -j DROP
            sudo iptables -t filter -A OUTPUT -p icmp -j DROP
            printf "Block ICMP\n"
            ;;

        block)
            # Bloc everything by default
            sudo iptables -t filter -P INPUT DROP
            sudo iptables -t filter -P FORWARD DROP
            sudo iptables -t filter -P OUTPUT DROP
            printf "Block all trafic\n"
            ;;

        restore)
            # Empty all rules
            sudo iptables -F
            sudo iptables -X

            sudo iptables -t nat -F
            sudo iptables -t nat -X
            sudo iptables -t mangle -F
            sudo iptables -t mangle -X
            sudo iptables -t raw -F
            sudo iptables -t raw -X
            sudo iptables -t security -F
            sudo iptables -t security -X

            # Restore rules
            sudo iptables -P INPUT ACCEPT
            sudo iptables -P FORWARD ACCEPT
            sudo iptables -P OUTPUT ACCEPT

            printf "we restored all rules\n"
            ;;

        *)
            printf "Possible OPTIONS :  restore\n                                       block\n"
            ;;
    esac

fi

#
#
# # Authorize already established connexions
# iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
# iptables -A OUTPUT -m state --state RELATED,ESTABLISHED -j ACCEPT
# iptables -t filter -A INPUT -i lo -j ACCEPT
# iptables -t filter -A OUTPUT -o lo -j ACCEPT
#
# # ICMP (Ping)
# iptables -t filter -A INPUT -p icmp -j ACCEPT
# iptables -t filter -A OUTPUT -p icmp -j ACCEPT
#
# # SSH
# iptables -t filter -A INPUT -p tcp --dport 22 -j ACCEPT
# iptables -t filter -A OUTPUT -p tcp --dport 22 -j ACCEPT
#
# # DNS
# iptables -t filter -A OUTPUT -p tcp --dport 53 -j ACCEPT
# iptables -t filter -A OUTPUT -p udp --dport 53 -j ACCEPT
# iptables -t filter -A INPUT -p tcp --dport 53 -j ACCEPT
# iptables -t filter -A INPUT -p udp --dport 53 -j ACCEPT
#
# # HTTP
# iptables -t filter -A OUTPUT -p tcp --dport 80 -j ACCEPT
# iptables -t filter -A INPUT -p tcp --dport 80 -j ACCEPT
#
# #HTTPS
# iptables -t filter -A OUTPUT -p tcp --dport 443 -j ACCEPT
# iptables -t filter -A INPUT -p tcp --dport 443 -j ACCEPT
#
# # FTP
# iptables -t filter -A OUTPUT -p tcp --dport 20:21 -j ACCEPT
# iptables -t filter -A INPUT -p tcp --dport 20:21 -j ACCEPT
#
# # Git
# iptables -t filter -A OUTPUT -p tcp --dport 9418 -j ACCEPT
# iptables -t filter -A INPUT -p tcp --dport 9418 -j ACCEPT
#
# # Mail SMTP
# iptables -t filter -A INPUT -p tcp --dport 25 -j ACCEPT
# iptables -t filter -A OUTPUT -p tcp --dport 25 -j ACCEPT
#
# # Mail POP3
# iptables -t filter -A INPUT -p tcp --dport 110 -j ACCEPT
# iptables -t filter -A OUTPUT -p tcp --dport 110 -j ACCEPT
#
# # Mail IMAP
# iptables -t filter -A INPUT -p tcp --dport 143 -j ACCEPT
# iptables -t filter -A OUTPUT -p tcp --dport 143 -j ACCEPT
#
# # NTP (server time)
# iptables -t filter -A OUTPUT -p udp --dport 123 -j ACCEPT
