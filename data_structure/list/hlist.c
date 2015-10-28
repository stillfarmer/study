#include <stdio.h>

/* add by wudi start, 2015/10/19 */
#define HASH_KEY(x) (x[SIX_MAC_ADDR_LEN - 1])
#define STR_SSID_LEN       40
#define MAX_GROUP_MATE_NUM 100
#define STA_LIST_NUM       256

void get_roam_table_info( void);

struct sta_node
{
	unsigned char       ap_mac[SIX_MAC_ADDR_LEN];
	unsigned char       sta_mac[SIX_MAC_ADDR_LEN];
	char                ssid_name[STR_SSID_LEN];
	unsigned long       time_long;
	struct  hlist_node  h_node;
};


struct hlist_head sta_list[STA_LIST_NUM];

void get_roam_table_info( void)
{
    int i = 0;
    unsigned int h_key = 0;
    struct sta_node    *p_sta_node = NULL;
    struct hlist_node  *p_pos_hlist_node = NULL;
    struct hlist_node  *p_n_hlist_node = NULL;

    cmsLog_debug("Enter");

    /* traversal the hash table */
    for ( h_key = 0; h_key < STA_LIST_NUM; h_key++)
    {   
        hlist_for_each_entry_safe(p_sta_node, p_pos_hlist_node, p_n_hlist_node, &sta_list[h_key], h_node)
        {
            if ( 0 == memcmp(p_sta_node->ap_mac, ap_mac, SIX_MAC_ADDR_LEN))
            {
                cmsLog_debug(" sta mac = %02x:%02x:%02x:%02x:%02x:%02x", 
                        p_sta_node->sta_mac[0], p_sta_node->sta_mac[1], p_sta_node->sta_mac[2],
                        p_sta_node->sta_mac[3], p_sta_node->sta_mac[4], p_sta_node->sta_mac[5]);

                cmsLog_debug(" sta ssid_name = %s",      p_sta_node->ssid_name);
                cmsLog_debug(" sta time_long = %lu\n\n", p_sta_node->time_long);
            }
        }
    }  

    cmsLog_debug("Exit");
    return;
}

void clean_sta_table( void)
{
	struct sta_node    *p_sta_node = NULL;
	struct hlist_node  *p_pos_hlist_node = NULL;
	struct hlist_node  *p_n_hlist_node = NULL;

    unsigned int h_key = 0;

	cmsLog_debug("Enter");

    for ( h_key = 0; h_key < STA_LIST_NUM; h_key++)
	{
		hlist_for_each_entry_safe(p_sta_node, p_pos_hlist_node, p_n_hlist_node, &sta_list[h_key], h_node)
		{
			hlist_del_init(&(p_sta_node->h_node));
			CMSMEM_FREE_BUF_AND_NULL_PTR(p_sta_node);
		}
	}

	cmsLog_debug("Exit");
}

/* get sta time long */
void check_sta_time( unsigned char *p_sta_mac_uch, char *p_ssid_name_ch, unsigned long *p_time_ulong)
{
	struct sysinfo info;

    unsigned int h_key = HASH_KEY(p_sta_mac_uch);

	struct       sta_node    *p_sta_node = NULL;
	struct       hlist_node  *p_pos_hlist_node = NULL;
	struct       hlist_node  *p_n_hlist_node = NULL;


	if ( (NULL == p_sta_mac_uch)
      || (NULL == p_ssid_name_ch)
      || (NULL == p_time_ulong)
        )
	{
	    cmsLog_error("Null pointer, error!");
		return;
	}

	if ( 0 != sysinfo(&info))
	{
	    cmsLog_error("get info failed, error!");
		return;
	}

	*p_time_ulong = 0;

	cmsLog_error("h_key = %u", h_key);

	hlist_for_each_entry_safe(p_sta_node, p_pos_hlist_node, p_n_hlist_node, &sta_list[h_key], h_node)
	{
	    if ( 0 == memcmp(p_sta_node->sta_mac, p_sta_mac_uch, SIX_MAC_ADDR_LEN))
		{
	    	if ( 0 == cmsUtl_strncmp(p_sta_node->ssid_name, p_ssid_name_ch, STR_SSID_LEN))
			{
				if ( info.uptime > p_sta_node->time_long)
				{
					hlist_del_init(&(p_sta_node->h_node));
					CMSMEM_FREE_BUF_AND_NULL_PTR(p_sta_node);
				}
				
				else
				{
			    	*p_time_ulong = p_sta_node->time_long - info.uptime;
				}
			}

			break;
		}
	}
}

/* before update sta table, refresh to del overtime entry */
void refresh_sta_table( unsigned char *p_sta_mac_uch)
{
	struct sta_node    *p_sta_node = NULL;
	struct hlist_node  *p_pos_hlist_node = NULL;
	struct hlist_node  *p_n_hlist_node = NULL;

	struct sysinfo info;

    unsigned int h_key = HASH_KEY(p_sta_mac_uch);

	cmsLog_debug("Enter");

	if ( 0 != sysinfo(&info))
	{
	    cmsLog_error("get info failed, error!");
		return;
	}

	hlist_for_each_entry_safe( p_sta_node, p_pos_hlist_node, p_n_hlist_node, &sta_list[h_key], h_node)
	{
	    if ( info.uptime > p_sta_node->time_long)
		{
			hlist_del_init(&(p_sta_node->h_node));
			CMSMEM_FREE_BUF_AND_NULL_PTR(p_sta_node);
		}
	}

	cmsLog_debug("Exit");
}

/* update include add, del, update the node */
void update_sta_table( unsigned char *p_sta_mac_uch, unsigned char *p_ap_mac_uch, char *p_ssid_name_ch, unsigned long *p_time_ulong)
{

	struct sta_node    *p_sta_node = NULL;
	struct hlist_node  *p_pos_hlist_node = NULL;
	struct hlist_node  *p_n_hlist_node = NULL;

	unsigned char is_found = 0;

	struct sysinfo info;

    unsigned int h_key = HASH_KEY(p_sta_mac_uch);

	cmsLog_debug("Enter");

	if ( (NULL == p_sta_mac_uch)
      || (NULL == p_ap_mac_uch)
      || (NULL == p_ssid_name_ch)
      || (NULL == p_time_ulong)
        )
	{
	    cmsLog_error("Null pointer, error!");
		return;
	}

	if ( 0 != sysinfo(&info))
	{
	    cmsLog_error("get info failed, error!");
		return;
	}

	hlist_for_each_entry_safe( p_sta_node, p_pos_hlist_node, p_n_hlist_node, &sta_list[h_key], h_node)
	{
	    if ( 0 == memcmp(p_sta_node->sta_mac, p_sta_mac_uch, SIX_MAC_ADDR_LEN))
		{
		    is_found = 1;
		    /* update the node */
		    if (  0 != (*p_time_ulong))
			{
				memcpy(p_sta_node->ap_mac, p_ap_mac_uch, SIX_MAC_ADDR_LEN);
				cmsUtl_strncpy(p_sta_node->ssid_name, p_ssid_name_ch, STR_SSID_LEN);
				p_sta_node->time_long = info.uptime + (*p_time_ulong);
			}
      
	        /* delete the node*/
			else
			{
			    hlist_del_init(&(p_sta_node->h_node));
				CMSMEM_FREE_BUF_AND_NULL_PTR(p_sta_node);
			}
		}
	}

    /* add the node */
	if ( (0 == is_found)
	     && ( 0 != (*p_time_ulong)))
	{
	    if ( NULL == ( p_sta_node = cmsMem_alloc(sizeof(struct sta_node), ALLOC_ZEROIZE)))
		{
		    cmsLog_error( " malloc error!");
			return;
		}

		memcpy(p_sta_node->ap_mac, p_ap_mac_uch, SIX_MAC_ADDR_LEN);
		memcpy(p_sta_node->sta_mac, p_sta_mac_uch, SIX_MAC_ADDR_LEN);
	    cmsUtl_strncpy(p_sta_node->ssid_name, p_ssid_name_ch, STR_SSID_LEN);
		p_sta_node->time_long = info.uptime + (*p_time_ulong);

		hlist_add_head( &(p_sta_node->h_node), &sta_list[h_key]);
	}

	cmsLog_debug("Exit");

	return;
}

void roam_init(void)
{
	int i = 0;

    for ( i = 0; i < STA_LIST_NUM; i++)
	{
	    sta_list[i].first = NULL;
	}
	
	return;
}
/* add by wudi end, 2015/10/19 */
