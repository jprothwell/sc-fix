/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2005
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE. 
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/



#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "kal_release.h"

#include "app_url.h"


static applib_scheme_struct supported_schemes[] = APPLIB_URL_SUPPORTED_SCHEMES;


#define ISALPHA(c)      (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
    
#define ISNUM(c)        ((c >= '0') && (c <= '9'))

static int _strncasecmp(const char *s1, const char *s2, unsigned int n)
{
    if (n == 0)
        return 0;

    while ((n-- != 0)
           && (tolower(*(unsigned char *) s1) ==
               tolower(*(unsigned char *) s2))) {
        if (n == 0 || *s1 == '\0' || *s2 == '\0')
            return 0;
        s1++;
        s2++;
    }

    return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}


static url_scheme_type_enum applib_url_get_scheme_to_type(const char *url, int len)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i;
    
    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	if(!url || len == 0)
	{
		return applib_scheme_empty;
	}

	for(i = 0; i < APPLIB_NBR_OF_SUPPORTED_SCHEMES; i++)
	{
		if(_strncasecmp(url, supported_schemes[i].leading_string, len) == 0)
		{
			return supported_schemes[i].scheme_type;
		}
	}

	return applib_scheme_unknown;
}


/*****************************************************************************
 * FUNCTION
 *  applib_url_check_scheme
 * DESCRIPTION
 *  
 * PARAMETERS
 *  url         [IN]
 *  st          [OUT]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_url_check_scheme(applib_url_struct *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    int i, len;
    const char *scheme;
    char ch;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." ) */
    
    scheme = p->parts[APPLIB_URL_SCHEME_PART];
    len = p->len[APPLIB_URL_SCHEME_PART];
    
    /* Scheme cannot be empty */
    if(!scheme || len == 0)
    {
        return APPLIB_URL_ERR_INVALID_SCHEME;
    }

    if(!ISALPHA(*scheme))
    {
        return APPLIB_URL_ERR_INVALID_SCHEME;
    }

    for(i = 0; i < len; i++)
    {
        ch = scheme[i];
        if(!ISALPHA(ch) && !ISNUM(ch) && (ch != '+') && (ch != '-') && (ch != '.'))
        {
            return APPLIB_URL_ERR_INVALID_SCHEME;
        }
    }
    
	/*if((p->scheme_type == applib_scheme_empty) ||
		(p->scheme_type == applib_scheme_unknown))
	{
		return APPLIB_URL_ERR_INVALID_SCHEME;
	}*/
	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  applib_url_check_host
 * DESCRIPTION
 *  
 * PARAMETERS
 *  url         [IN]
 *  st          [OUT]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_url_check_host(applib_url_struct *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/
	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  applib_url_check_port
 * DESCRIPTION
 *  
 * PARAMETERS
 *  url         [IN]
 *  st          [OUT]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_url_check_port(applib_url_struct *p)
{
	char port[6] = "";

	strncpy(port, p->parts[APPLIB_URL_PORT_PART], (p->len[APPLIB_URL_PORT_PART] < 5) ? p->len[APPLIB_URL_PORT_PART] : 5);

	if(atoi(port) < 0 || atoi(port) > 65535)
	{
		return APPLIB_URL_ERR_INVALID_PORT;
	}
	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  app_parse_url
 * DESCRIPTION
 *  
 * PARAMETERS
 *  url         [IN]
 *  st          [OUT]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_parse_url(const char *url, applib_url_struct *p)
{
    /*----------------------------------------------------------------*/
    /* Local Variables                                                */
    /*----------------------------------------------------------------*/
    const char *next, *iter;
    char ch = 0;
    int i;

    /*----------------------------------------------------------------*/
    /* Code Body                                                      */
    /*----------------------------------------------------------------*/

    /* Basecase check */    
    if(!url || !p)
    {
        return -1;
    }

    /* Clear the structure first */
    {
        p->scheme_type = applib_scheme_empty;
        for(i = 0; i < APPLIB_URL_NUM_PARTS; i++)
        {
            p->parts[i] = NULL;
            p->len[i] = 0;
        }
    }
    
    next = url; /* start from the beginning of the URL */

    /* URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ] */

    /* Extract the scheme part */
    for(iter = next; *iter; iter++)
    {
        ch = *iter;
        if (ch == ':')
		{
			break;
		}
	}

	if(ch == ':')
    {
        p->parts[APPLIB_URL_SCHEME_PART] = next;
        p->len[APPLIB_URL_SCHEME_PART] = iter - next;
        next = iter + 1;
        p->scheme_type = applib_url_get_scheme_to_type(p->parts[APPLIB_URL_SCHEME_PART], p->len[APPLIB_URL_SCHEME_PART]);
    }

    /* Parse autority part
       authority     = [ userinfo "@" ] host [ ":" port ] */
	if(!((next[0] == '/') && (next[1] == '/')))
    {
        goto path; /* No authority part */
    }

	/* Skip "//" */
	next += 2;
    /* Extract userinfo part */
    for(iter = next; *iter; iter++)
    {
        ch = *iter;
        if (ch == '@')
		{
			break;
		}
	}

    if(ch == '@')
    {
        p->parts[APPLIB_URL_USERINFO_PART] = next;
        p->len[APPLIB_URL_USERINFO_PART] = iter - next;
        next = iter + 1;
    }

    /* Extract host part */
    for(iter = next; *iter; iter++)
    {
        ch = *iter;
        if ((ch == ':') || (ch == '/'))
		{
			break;
		}
	}

	p->parts[APPLIB_URL_HOST_PART] = next;
	p->len[APPLIB_URL_HOST_PART] = iter - next;

	/* Reach the end of string */
	if(*iter == 0)
	{
		goto end;
	}
	
	next = iter + 1;

	if(ch == '/')
	{
		/* There is no port part */
		goto path;
	}

    if(ch == ':')
    {
        /* Extract port part */
        for(iter = next; *iter; iter++)
        {
            ch = *iter;
            if (ch == '/')
			{
				break;
			}
		}

        p->parts[APPLIB_URL_PORT_PART] = next;
        p->len[APPLIB_URL_PORT_PART] = iter - next;

        if(ch == '/')
        {
            next = iter + 1;
        }
        else /* Reach the end of URL */
        {
            goto end;
        }
    }

path:
    /* Extract path part */
    for(iter = next; *iter; iter++)
    {
        ch = *iter;
		if((ch == '?') || (ch == '#'))
		{
			break;
		}
	}
    
	p->parts[APPLIB_URL_PATH_PART] = next;
	p->len[APPLIB_URL_PATH_PART] = iter - next;
    if((ch == '?') || (ch == '#'))
    {
        next = iter + 1;
    }
    else
    {
        goto end;
    }

    if(ch == '#')
    {
		/* There is no query part */
        goto fragment;
    }

    /* Extract query part */
    if(ch == '?')
    {
        for(iter = next; *iter; iter++)
        {
            ch = *iter;
            if (ch == '#')
              break;
        }

        p->parts[APPLIB_URL_QUERY_PART] = next;
		p->len[APPLIB_URL_QUERY_PART] = iter - next;
        if(ch == '#')
        {            
            next = iter + 1;
        }
        else
        {
            goto end;
        }
    }        

fragment:
    for(iter = next; *iter; iter++);
    p->parts[APPLIB_URL_FRAGMENT_PART] = next;
	p->len[APPLIB_URL_FRAGMENT_PART] = iter - next;

end:
	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  applib_url_is_valid
 * DESCRIPTION
 *  To check if a URL is valid or not.
 * PARAMETERS
 *  url         [IN]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_url_is_valid(const char *url)
{
	applib_url_struct st;
	int ret;

	ret = applib_parse_url(url, &st);
	if(ret < 0)
	{
		return ret;
	}

	ret = applib_url_check_scheme(&st);
	if(ret < 0)
	{
		return ret;
	}

	if(st.scheme_type == applib_scheme_unknown)
	{
		return APPLIB_URL_ERR_INVALID_SCHEME;
	}

	ret = applib_url_check_host(&st);
	if(ret < 0)
	{
		return ret;
	}

	ret = applib_url_check_port(&st);
	if(ret < 0)
	{
		return ret;
	}

	return 0;
}


/*****************************************************************************
 * FUNCTION
 *  applib_url_is_valid
 * DESCRIPTION
 *  To check if a URL is valid or not with assigned schemes.
 * PARAMETERS
 *  url         [IN]
 * RETURNS
 *  0 if success
 *****************************************************************************/
int applib_url_is_valid_ext(const char *url, unsigned short schemes)
{
	applib_url_struct st;
	int ret;

	ret = applib_parse_url(url, &st);
	if(ret < 0)
	{
		return ret;
	}

	ret = applib_url_check_scheme(&st);
	if(ret < 0)
	{
		return ret;
	}

    if(st.scheme_type == applib_scheme_unknown || !(st.scheme_type & schemes))
    {
        return APPLIB_URL_ERR_INVALID_SCHEME;
    }

	ret = applib_url_check_host(&st);
	if(ret < 0)
	{
		return ret;
	}

	ret = applib_url_check_port(&st);
	if(ret < 0)
	{
		return ret;
	}

	return 0;
}


