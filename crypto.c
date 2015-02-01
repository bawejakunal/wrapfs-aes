/* 
 * This file implements all functions needed for encryption of data in file system
 * Author: Kunal Baweja
 *
 */

#include "wrapfs.h"

 const char zero_key[KEYLEN]="0000000000000000";

 /* Calculates MD5 hash of they key value 
  * @key: string of user passed key value
  * @len: length of key passed
  */

 long calculate_md5(char *src, char *dest, int len)
 {
 	int err = 0;
 	struct scatterlist sg;
 	struct crypto_hash *tfm = NULL;
	struct hash_desc desc;

	tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if(IS_ERR(tfm))
		return -EINVAL;

	desc.tfm = tfm;
	desc.flags = 0;
	sg_init_one(&sg, (u8 *)src, KEYLEN);
	if (!desc.tfm) 
	{
		desc.tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
		if (IS_ERR(desc.tfm)) 
		{
			err = PTR_ERR(desc.tfm);
			printk(KERN_ERR "Error attempting to "
					"allocate crypto context; err = [%d]\n",
					err);
			goto out;
		}
	}

	crypto_hash_setkey(tfm, (u8 *)src, KEYLEN);
	err = crypto_hash_init(&desc);
	if (err) {
		printk(KERN_ERR
		       "%s: Error initializing crypto hash; err = [%d]\n",
		       __func__, err);
		goto out;
	}

	err = crypto_hash_update(&desc, &sg, len);
	if (err) {
		printk(KERN_ERR
		       "%s: Error updating crypto hash; err = [%d]\n",
		       __func__, err);
		goto out;
	}

	err = crypto_hash_final(&desc, dest);
	if (err) {
		printk(KERN_ERR
		       "%s: Error finalizing crypto hash; err = [%d]\n",
		       __func__, err);
		goto out;
	}

out:
	crypto_free_hash(desc.tfm);
	return err;
 }


/* function to encrypt data, returns 0 on success */
int encrypt_decrypt(void *key, void *dest, const void *src, size_t len, int option)
{
	int err = 0;
	struct crypto_blkcipher *tfm;
	struct blkcipher_desc desc;
	struct scatterlist sg_in, sg_out;

	if(0 == memcmp(zero_key, key, KEYLEN))
	{
		err = -ENOKEY;
		goto out;
	}

	tfm = crypto_alloc_blkcipher("ctr(aes)",0,CRYPTO_ALG_ASYNC);

	if(IS_ERR(tfm))
	{
		err = PTR_ERR(tfm);
		goto out;
	}

	desc.tfm = tfm;
	desc.flags = 0;

	err = crypto_blkcipher_setkey(tfm, (u8* )key, KEYLEN);
	if(err)
	{
		pr_err("Encryption: setkey failed: %d",err);
		goto out_tfm;
	}

	sg_init_one(&sg_in, src, len);
	sg_init_one(&sg_out, dest, len);

	/* option = 1 for encryption */
	if(option)
		err = crypto_blkcipher_encrypt(&desc, &sg_out, &sg_in, len);
	else
		err = crypto_blkcipher_decrypt(&desc, &sg_out, &sg_in, len);

	if(err < 0)
		pr_err("Wrapfs: Encryption/Decryption failed %d",err);

out_tfm:
	crypto_free_blkcipher(tfm);
out:
	return err;
}