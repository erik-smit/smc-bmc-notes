/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH &lt;www.elinos.com&gt;
 * Marius Groeger &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">mgroeger at sysgo.de</A>&gt;
 *
 * Copyright (C) 2006 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>&gt;
 * Macpaul Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>&gt;
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include &lt;asm/andesboot.h&gt;

static int check_crc(bd_t *bd)
{
	/* need to calculate crc? */
	if (bd-&gt;bi_ext.env_crc_valid == 0)
	{
		env_t *env = (env_t *)CONFIG_ENV_ADDR;

		if (crc32(0, env-&gt;data, sizeof(env-&gt;data)) == env-&gt;crc)
			bd-&gt;bi_ext.env_crc_valid = 1;
		else
			bd-&gt;bi_ext.env_crc_valid = -1;
	}
	return bd-&gt;bi_ext.env_crc_valid &gt; 0;
}


/*
 * save environment buffer back to flash
 * returns -1 on error, 0 if ok
 */
int board_env_save(bd_t *bd, env_t *env, int size)
{
	int rc;
	ulong start_addr, end_addr;


//no CONFIG_ENV_ADDR_REDUND
#if CONFIG_ENV_SIZE &gt; CONFIG_ENV_SECT_SIZE /* modified: &lt;= to &gt;, by Hill, 20090313 */
#error Make sure that CONFIG_ENV_SIZE &lt;= CONFIG_ENV_SECT_SIZE
#endif

	start_addr = CONFIG_ENV_ADDR;
	end_addr   = start_addr + CONFIG_ENV_SIZE - 1;

	rc = flash_sect_protect(0, CONFIG_ENV_ADDR, end_addr);
	if (rc &lt; 0)
		return rc;

	rc = flash_sect_erase(start_addr, end_addr);
	if (rc &lt; 0) {
		flash_sect_protect(1, start_addr, end_addr);
		flash_perror(rc);
		return rc;
	}

	printf(&quot;Saving Environment to Flash...&quot;);
	rc = flash_write((uchar*)env, start_addr, size);
	if (rc &lt; 0)
		flash_perror(rc);
	else
		printf(&quot;done.\n&quot;);

	(void)flash_sect_protect(1, start_addr, end_addr);

	return 0;
}

/*
 * copy environment to memory
 * returns -1 on error, 0 if ok
 */
int board_env_copy(bd_t *bd, env_t *data, int size)
{
	env_t *env = (env_t *)CONFIG_ENV_ADDR;

	if (check_crc(bd)) {
		memcpy(data, env, sizeof(env_t));
		return 0;
	}

	return -1;
}

/*
 * try to read env character at offset #index
 *
 * called before the environment is copied to ram
 * returns -1 on error, 0 if ok
 */
int board_env_getchar(bd_t * bd, int index, uchar *c)
{
	env_t *env = (env_t *)CONFIG_ENV_ADDR;

	/* check environment crc */
	if (index &lt; sizeof(env-&gt;data) &amp;&amp; check_crc(bd)) {
		*c = env-&gt;data[index];
		return 0;
	}

	return -1;
}

/*
 * try to read env character at offset #index
 *
 * called before the environment is copied to ram
 * returns -1 on error, 0 if ok
 */
uchar *board_env_getaddr(bd_t * bd, int index)
{
	env_t *env = (env_t *)CONFIG_ENV_ADDR;

	/* check environment crc */
	if (index &lt; sizeof(env-&gt;data) &amp;&amp; check_crc(bd))
		return &amp;env-&gt;data[index];

	return 0;
}
