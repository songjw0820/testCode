#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/if_tun.h>

#define DEFAULT_AWDL_DEVICE "awdl0"
#define log_error printf

int main() {

	static int one = 1;
	struct ifreq ifr;
	int fd, err, s;
	char host[IFNAMSIZ] = DEFAULT_AWDL_DEVICE;
	unsigned char  if_ether_addr[6];
	if_ether_addr[0]=00;
	if_ether_addr[1]=95;
	if_ether_addr[2]=69;
	if_ether_addr[3]=12;
	if_ether_addr[4]=34;
	if_ether_addr[5]=56;
	if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
		log_error("tun: unable to open tun device %d", fd);
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	/* Flags: IFF_TUN   - TUN device (no Ethernet headers)
	 *        IFF_TAP   - TAP device
	 *        IFF_NO_PI - Do not provide packet information
	 */
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	//if (*dev)
		strncpy(ifr.ifr_name, host, IFNAMSIZ);
	printf("host [%s] [%s]",ifr.ifr_name,host);
	if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
		close(fd);
		printf("TUNSETIFF err \n");
		return err;
	}
	strcpy(host, ifr.ifr_name);

	/* Set non-blocking mode */
	if ((err = ioctl(fd, FIONBIO, &one)) < 0) {
		close(fd);
		printf("FIONBIO err \n");
		return err;
	}

	// Create a socket for ioctl
	s = socket(AF_INET6, SOCK_DGRAM, 0);

	// Set HW address
	ifr.ifr_hwaddr.sa_family = 1; /* ARPHRD_ETHER */
	memcpy(ifr.ifr_hwaddr.sa_data, if_ether_addr, 6);
	if ((err = ioctl(s, SIOCSIFHWADDR, &ifr)) < 0) {
		log_error("tun: unable to set HW address");
		close(fd);
		return err;
	}

	// Get current flags and set them
	ioctl(s, SIOCGIFFLAGS, &ifr);
	ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
	if ((err = ioctl(s, SIOCSIFFLAGS, &ifr)) < 0) {
		log_error("tun: unable to set up");
		close(fd);
		return err;
	}

	/* Set reduced MTU */
	ifr.ifr_mtu = 1450; /* TODO arbitary limit to fit all headers */
	if ((err = ioctl(s, SIOCSIFMTU, (void *) &ifr)) < 0) {
		log_error("tun: unable to set MTU");
		close(fd);
		return err;
	}

	close(s);
	while(1){
		printf("test loop\n");
		sleep(1);
	}
	return fd;
}
