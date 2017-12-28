#ifndef __CAP_H_
#define __CAP_H_

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PCAP_VERSION_MAJOR 2
#define PCAP_VERSION_MINOR 4
#define PCAP_ERRBUF_SIZE 256

//bpf filter
typedef	int bpf_int32;
typedef	u_int bpf_u_int32;
struct bpf_program {
    u_int bf_len;
    struct bpf_insn *bf_insns;
};

typedef struct pcap pcap_t;
typedef struct pcap_dumper pcap_dumper_t;
typedef struct pcap_if pcap_if_t;
typedef struct pcap_addr pcap_addr_t;

/*
 * The first record in the file contains saved values for some
 * of the flags used in the printout phases of tcpdump.
 * Many fields here are 32 bit ints so compilers won't insert unwanted
 * padding; these files need to be interchangeable across architectures.
 *
 * Do not change the layout of this structure, in any way (this includes
 * changes that only affect the length of fields in this structure).
 *
 * Also, do not change the interpretation of any of the members of this
 * structure, in any way (this includes using values other than
 * LINKTYPE_ values, as defined in "savefile.c", in the "linktype"
 * field).
 *
 * Instead:
 *
 *	introduce a new structure for the new format, if the layout
 *	of the structure changed;
 *
 *	send mail to "tcpdump-workers@lists.tcpdump.org", requesting
 *	a new magic number for your new capture file format, and, when
 *	you get the new magic number, put it in "savefile.c";
 *
 *	use that magic number for save files with the changed file
 *	header;
 *
 *	make the code in "savefile.c" capable of reading files with
 *	the old file header as well as files with the new file header
 *	(using the magic number to determine the header format).
 *
 * Then supply the changes as a patch at
 *
 *	http://sourceforge.net/projects/libpcap/
 *
 * so that future versions of libpcap and programs that use it (such as
 * tcpdump) will be able to read your new capture file format.
 */
struct pcap_file_header {
	bpf_u_int32 magic;
	u_short version_major;
	u_short version_minor;
	bpf_int32 thiszone;	/* gmt to local correction */
	bpf_u_int32 sigfigs;	/* accuracy of timestamps */
	bpf_u_int32 snaplen;	/* max length saved portion of each pkt */
	bpf_u_int32 linktype;	/* data link type (LINKTYPE_*) */
};

/*
 * Macros for the value returned by pcap_datalink_ext().
 * 
 * If LT_FCS_LENGTH_PRESENT(x) is true, the LT_FCS_LENGTH(x) macro
 * gives the FCS length of packets in the capture.
 */
#define LT_FCS_LENGTH_PRESENT(x)	((x) & 0x04000000)
#define LT_FCS_LENGTH(x)		(((x) & 0xF0000000) >> 28)
#define LT_FCS_DATALINK_EXT(x)		((((x) & 0xF) << 28) | 0x04000000)

typedef enum {
       PCAP_D_INOUT = 0,
       PCAP_D_IN,
       PCAP_D_OUT
} pcap_direction_t;

/*
 * Generic per-packet information, as supplied by libpcap.
 *
 * The time stamp can and should be a "struct timeval", regardless of
 * whether your system supports 32-bit tv_sec in "struct timeval",
 * 64-bit tv_sec in "struct timeval", or both if it supports both 32-bit
 * and 64-bit applications.  The on-disk format of savefiles uses 32-bit
 * tv_sec (and tv_usec); this structure is irrelevant to that.  32-bit
 * and 64-bit versions of libpcap, even if they're on the same platform,
 * should supply the appropriate version of "struct timeval", even if
 * that's not what the underlying packet capture mechanism supplies.
 */
struct pcap_pkthdr {
	struct timeval ts;	/* time stamp */
	bpf_u_int32 caplen;	/* length of portion present */
	bpf_u_int32 len;	/* length this packet (off wire) */
};

//As returned by the pcap_stats()
struct pcap_stat {
	u_int ps_recv;		/* number of packets received */
	u_int ps_drop;		/* number of packets dropped */
	u_int ps_ifdrop;	/* drops by interface -- only supported on some platforms */
};

//Item in a list of interfaces.
struct pcap_if {
	struct pcap_if *next;
	char *name;		/* name to hand to "pcap_open_live()" */
	char *description;	/* textual description of interface, or NULL */
	struct pcap_addr *addresses;
	bpf_u_int32 flags;	/* PCAP_IF_ interface flags */
};

#define PCAP_IF_LOOPBACK	0x00000001	/* interface is loopback */

//Representation of an interface address.
struct pcap_addr {
	struct pcap_addr *next;
	struct sockaddr *addr;		/* address */
	struct sockaddr *netmask;	/* netmask for that address */
	struct sockaddr *broadaddr;	/* broadcast address for that address */
	struct sockaddr *dstaddr;	/* P2P destination address for that address */
};

//回调函数类型定义
typedef void (*pcap_handler)(u_char *, const struct pcap_pkthdr *,
			     const u_char *);

//为serv框架服务的读接口
int pcap_read_packet(pcap_t *p, int cnt, pcap_handler callback, u_char *read_buf);
int pcap_offline_read(pcap_t *p, int cnt, pcap_handler callback, u_char *read_buf);

//Error codes for the pcap API.
#define PCAP_ERROR			-1	/* generic error code */
#define PCAP_ERROR_BREAK		-2	/* loop terminated by pcap_breakloop */
#define PCAP_ERROR_NOT_ACTIVATED	-3	/* the capture needs to be activated */
#define PCAP_ERROR_ACTIVATED		-4	/* the operation can't be performed on already activated captures */
#define PCAP_ERROR_NO_SUCH_DEVICE	-5	/* no such device exists */
#define PCAP_ERROR_RFMON_NOTSUP		-6	/* this device doesn't support rfmon (monitor) mode */
#define PCAP_ERROR_NOT_RFMON		-7	/* operation supported only in monitor mode */
#define PCAP_ERROR_PERM_DENIED		-8	/* no permission to open the device */
#define PCAP_ERROR_IFACE_NOT_UP		-9	/* interface isn't up */
#define PCAP_ERROR_CANTSET_TSTAMP_TYPE	-10	/* this device doesn't support setting the time stamp type */
#define PCAP_ERROR_PROMISC_PERM_DENIED	-11	/* you don't have permission to capture in promiscuous mode */

//Warning codes for the pcap API.
#define PCAP_WARNING			1	/* generic warning code */
#define PCAP_WARNING_PROMISC_NOTSUP	2	/* this device doesn't support promiscuous mode */
#define PCAP_WARNING_TSTAMP_TYPE_NOTSUP	3	/* the requested time stamp type is not supported */

//Value to pass to pcap_compile() as the netmask if you don't know what the netmask is.
#define PCAP_NETMASK_UNKNOWN	0xffffffff

char *pcap_lookupdev(char *);
int	pcap_lookupnet(const char *, bpf_u_int32 *, bpf_u_int32 *, char *);

pcap_t	*pcap_create(const char *, char *);
int	pcap_set_snaplen(pcap_t *, int);
int	pcap_set_promisc(pcap_t *, int);
int	pcap_can_set_rfmon(pcap_t *);
int	pcap_set_rfmon(pcap_t *, int);
int	pcap_set_timeout(pcap_t *, int);
int	pcap_set_tstamp_type(pcap_t *, int);
int	pcap_set_buffer_size(pcap_t *, int);
int	pcap_activate(pcap_t *);

int	pcap_list_tstamp_types(pcap_t *, int **);
void pcap_free_tstamp_types(int *);
int	pcap_tstamp_type_name_to_val(const char *);
const char *pcap_tstamp_type_val_to_name(int);
const char *pcap_tstamp_type_val_to_description(int);

/*
 * Time stamp types.
 * Not all systems and interfaces will necessarily support all of these.
 *
 * A system that supports PCAP_TSTAMP_HOST is offering time stamps
 * provided by the host machine, rather than by the capture device,
 * but not committing to any characteristics of the time stamp;
 * it will not offer any of the PCAP_TSTAMP_HOST_ subtypes.
 *
 * PCAP_TSTAMP_HOST_LOWPREC is a time stamp, provided by the host machine,
 * that's low-precision but relatively cheap to fetch; it's normally done
 * using the system clock, so it's normally synchronized with times you'd
 * fetch from system calls.
 *
 * PCAP_TSTAMP_HOST_HIPREC is a time stamp, provided by the host machine,
 * that's high-precision; it might be more expensive to fetch.  It might
 * or might not be synchronized with the system clock, and might have
 * problems with time stamps for packets received on different CPUs,
 * depending on the platform.
 *
 * PCAP_TSTAMP_ADAPTER is a high-precision time stamp supplied by the
 * capture device; it's synchronized with the system clock.
 *
 * PCAP_TSTAMP_ADAPTER_UNSYNCED is a high-precision time stamp supplied by
 * the capture device; it's not synchronized with the system clock.
 *
 * Note that time stamps synchronized with the system clock can go
 * backwards, as the system clock can go backwards.  If a clock is
 * not in sync with the system clock, that could be because the
 * system clock isn't keeping accurate time, because the other
 * clock isn't keeping accurate time, or both.
 *
 * Note that host-provided time stamps generally correspond to the
 * time when the time-stamping code sees the packet; this could
 * be some unknown amount of time after the first or last bit of
 * the packet is received by the network adapter, due to batching
 * of interrupts for packet arrival, queueing delays, etc..
 */
#define PCAP_TSTAMP_HOST		        0	/* host-provided, unknown characteristics */
#define PCAP_TSTAMP_HOST_LOWPREC	    1	/* host-provided, low precision */
#define PCAP_TSTAMP_HOST_HIPREC		    2	/* host-provided, high precision */
#define PCAP_TSTAMP_ADAPTER		        3	/* device-provided, synced with the system clock */
#define PCAP_TSTAMP_ADAPTER_UNSYNCED	4	/* device-provided, not synced with the system clock */

pcap_t	*pcap_open_live(const char *, int, int, int, char *);
pcap_t	*pcap_open_dead(int, int);
pcap_t	*pcap_open_offline(const char *, char *);
pcap_t	*pcap_fopen_offline(FILE *, char *);
int pcap_close_rfile(pcap_t *p);

void pcap_close(pcap_t *);
int pcap_loop(pcap_t *, int, pcap_handler, u_char *);
int	pcap_dispatch(pcap_t *, int, pcap_handler, u_char *);
const u_char *pcap_next(pcap_t *, struct pcap_pkthdr *);
int pcap_next_ex(pcap_t *, struct pcap_pkthdr **, const u_char *);
void pcap_breakloop(pcap_t *);
int	pcap_stats(pcap_t *, struct pcap_stat *);
int	pcap_setfilter(pcap_t *, struct bpf_program *);
int pcap_setdirection(pcap_t *, pcap_direction_t);
int	pcap_getnonblock(pcap_t *, char *);
int	pcap_setnonblock(pcap_t *, int, char *);
int	pcap_inject(pcap_t *, const void *, size_t);
int	pcap_sendpacket(pcap_t *, const u_char *, int);
const char *pcap_statustostr(int);
const char *pcap_strerror(int);
char *pcap_geterr(pcap_t *);
void pcap_perror(pcap_t *, char *);
int	pcap_compile(pcap_t *, struct bpf_program *, const char *, int, bpf_u_int32);
int	pcap_compile_nopcap(int, int, struct bpf_program *, const char *, 
        int, bpf_u_int32);
void pcap_freecode(struct bpf_program *);
int	pcap_offline_filter(struct bpf_program *, const struct pcap_pkthdr *,
	    const u_char *);
int	pcap_datalink(pcap_t *);
int	pcap_datalink_ext(pcap_t *);
int	pcap_list_datalinks(pcap_t *, int **);
int	pcap_set_datalink(pcap_t *, int);
void pcap_free_datalinks(int *);
int	pcap_datalink_name_to_val(const char *);
const char *pcap_datalink_val_to_name(int);
const char *pcap_datalink_val_to_description(int);
int	pcap_snapshot(pcap_t *);
int	pcap_is_swapped(pcap_t *);
int	pcap_major_version(pcap_t *);
int	pcap_minor_version(pcap_t *);

/* XXX */
FILE *pcap_file(pcap_t *);
int	pcap_fileno(pcap_t *);

pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);
pcap_dumper_t *pcap_dump_fopen(pcap_t *, FILE *fp);
FILE *pcap_dump_file(pcap_dumper_t *);
long pcap_dump_ftell(pcap_dumper_t *);
int	pcap_dump_flush(pcap_dumper_t *);
void pcap_dump_close(pcap_dumper_t *);
void pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *);

int	pcap_findalldevs(pcap_if_t **, char *);
void pcap_freealldevs(pcap_if_t *);

int	pcap_get_selectable_fd(pcap_t *);

//将NetBSD中的<net/bpf.h>框架移植至此
u_int	bpf_filter(const struct bpf_insn *, const u_char *, u_int, u_int);
int     bpf_validate(const struct bpf_insn *f, int len);
char	*bpf_image(const struct bpf_insn *, int);
void	bpf_dump(const struct bpf_program *, int);


#ifdef __cplusplus
}
#endif

#endif
