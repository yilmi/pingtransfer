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

void listener(struct protoent *proto)
{

    int sd;
    unsigned char buf[1500];

    sd = socket(AF_INET, SOCK_RAW, proto->p_proto);

    if (sd < 0)
    {
        perror("\tsocket error\n");
        exit(0);
    }

    //Disables printf buffering so payload is printed directly, other solutions for the same problem exist
    setbuf(stdout, NULL);

    for (;;)
    {
        int bytes = 0;
        bzero(buf, sizeof(buf));

        bytes = recvfrom(sd, buf, sizeof(buf), 0, NULL, NULL);
        if (bytes > 0)
        {
            printf("%s", (buf + 28));

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
    printf("Just run pingserver as root");
    exit(0);
}

int main()
{

    struct protoent *proto = getprotobyname("ICMP"); // Protocol type

    listener(proto);

    return 0;
}
