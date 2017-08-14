/* 
 * Use may be in whole or in part in accordance to the General Public License 
 * (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

/** Based on code from Sean Walton and Macmillan Publishers 2000 **/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

void listener(int payloadsize, struct sockaddr_in src_address, struct protoent *proto)
{

    int sd;
    unsigned char buf[1500];
    payloadsize = payloadsize - 4;

    sd = socket(AF_INET, SOCK_RAW, proto->p_proto);

    if (sd < 0)
    {
        perror("\tsocket error\n");
        exit(0);
    }


    //Disables printf buffering so payload is printed directly
    setbuf(stdout, NULL);

    for (;;)
    {

        int bytes, laddr_len = sizeof(src_address);
        int i;
        bzero(buf, sizeof(buf));

        bytes = recvfrom(sd, buf, sizeof(buf), 0, (struct sockaddr *)&src_address, &laddr_len);
        if (bytes > 0)
        {
            printf("%s", (buf + payloadsize));

        }
        else
        {
            perror("Receive Error\n");
        }
    }
    exit(0);
}

void usage()
{
    printf("Please provide remote ip address\n");
    printf("usage: pingserver <addr> <payload size>\n");
    exit(0);
}

int main(int argc, char *argv[])
{

    struct protoent *proto = getprotobyname("ICMP"); // Protocol type
    struct sockaddr_in addr;
    int payloadsize;
    char *argaddress;

    if (argc != 3 || argc < 2)
    {
        usage();
    }

    if (atoi(argv[2]))
    {
        payloadsize = atoi(argv[2]);
    }
    else
    {
        printf("Invalid payload size\n");
        usage();
    }

    addr.sin_family = AF_INET;
    addr.sin_port = 0;
    addr.sin_addr.s_addr = inet_addr(argv[1]);

    listener(payloadsize, addr, proto);

    return 0;
}
