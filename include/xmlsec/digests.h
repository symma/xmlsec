/** 
 * XMLSec library
 *
 *
 * See Copyright for the status of this software.
 * 
 * Author: Aleksey Sanin <aleksey@aleksey.com>
 */
#ifndef __XMLSEC_DIGESTS_H__
#define __XMLSEC_DIGESTS_H__    


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/transforms.h>
#include <xmlsec/transformsInternal.h>

typedef struct _xmlSecDigestTransform  *xmlSecDigestTransformPtr,
					xmlSecDigestTransform;
typedef const struct _xmlSecDigestTransformIdStruct	*xmlSecDigestTransformId;


/**
 * xmlSecDigestUpdateMethod:
 * @transform: the pointer to digests transform #xmlSecDigestTransform.
 * @buffer: the input data.
 * @size: the input data size.
 *
 * Updates digest with new chunk of data.
 *
 * Returns 0 on success or a negative value otherwise.
 */
typedef int (*xmlSecDigestUpdateMethod)		(xmlSecDigestTransformPtr transform,
						 const unsigned char *buffer,
						 size_t size);
/**
 * xmlSecDigestSignMethod:
 * @transform: the pointer to digests transform #xmlSecDigestTransform.
 * @buffer: the pointer to pointer to the output data.
 * @size: the pointer to outut data size.
 *
 * Finalizes digest and returns result in allocated buffer (@buffer, @size).
 * The caller is responsible for freeing returned buffer with xmlFree() function.
 *
 * Returns 0 on success or a negative value otherwise.
 */
typedef int (*xmlSecDigestSignMethod)		(xmlSecDigestTransformPtr transform,
						 unsigned char **buffer,
						 size_t *size);
/**
 * xmlSecDigestVerifyMethod:
 * @transform: the pointer to digests transform #xmlSecDigestTransform.
 * @buffer: the input data.
 * @size: the input data size.
 *
 * Verifies digest with new chunk of data. The actual verification
 * status is set in #status member of #xmlSecDigestTransform structure.
 *
 * Returns 1 on success or a negative value otherwise.
 */
typedef int (*xmlSecDigestVerifyMethod)		(xmlSecDigestTransformPtr transform,
						 const unsigned char *buffer,
						 size_t size);


/**
 * xmlSecDigestTransformId:
 * @type: the type.
 * @usage: the usage.
 * @href: the algorithm href.
 * @create: creation method.
 * @destroy: destroy method.
 * @read: xml node read method.
 * @keyId: the transform's key id.
 * @encryption: the key type (public/private) for encryption.
 * @decryption: the key type (public/private) for encryption.
 * @binSubType: the transform's binary sub type.
 * @addBinKey:  add key method.
 * @readBin: read binary data method.
 * @writeBin: write binary data method.
 * @flushBin: flush binary data method.
 * @digestUpdate: the update method.
 * @digestSign: the sizgn method.
 * @digestVerify: the verify method.
 * 
 * The digest transform id (%xmlSecTransformTypeBinary type).
 */ 
struct _xmlSecDigestTransformIdStruct {
    /* general data */
    const xmlChar*			name;
    xmlSecTransformType			type;
    xmlSecTransformUsage		usage;
    const xmlChar			*href;

    /* general methods */
    xmlSecTransformCreateMethod		create;
    xmlSecTransformDestroyMethod	destroy;
    xmlSecTransformNodeReadMethod	readNode;    
    xmlSecTransformSetKeyRequirements	setKeyReq;
    xmlSecTransformSetKeyMethod		setKey;
    
    /* binary methods */
    xmlSecTransformExecuteBinMethod	executeBin;

    xmlSecTransformReadMethod		readBin; 
    xmlSecTransformWriteMethod		writeBin;
    xmlSecTransformFlushMethod		flushBin;

    /* xml methods */
    xmlSecTransformExecuteXmlMethod	executeXml;

    /* c14n methods */
    xmlSecTransformExecuteC14NMethod	executeC14N;
    
    /* xmlSecDigestTransform data/methods */
    xmlSecDigestUpdateMethod		digestUpdate;
    xmlSecDigestSignMethod		digestSign;
    xmlSecDigestVerifyMethod		digestVerify;
};

/**
 * xmlSecDigestTransform:
 * @id: the transform id (pointer to #xmlSecTransformId).
 * @status: the transform status (ok/fail/unknown).
 * @dontDestroy: the don't automatically destroy flag.
 * @data: the pointer to transform specific data.
 * @encode: encode/decode (encrypt/decrypt) flag.
 * @next: next binary transform in the chain.
 * @prev: previous binary transform in the chain.
 * @binData: the pointer to binary transform speific data.
 * @pushModeEnabled: if 1 then the data are sent to next transform, otherwise
 * 	 we are keeping data for sign or verify call.
 * @digest: the internal digest buffer.
 * @digestSize: the internal digest buffer size.
 * @digestData: the pointer to digest transform specific data.
 *
 * The digests transform.
 */ 
struct _xmlSecDigestTransform {	
    /* general data */
    xmlSecTransformId 			id; 
    xmlSecTransformStatus		status;
    int					dontDestroy;

    /* binary specific */
    int					encode;
    xmlSecTransformPtr			next;
    xmlSecTransformPtr			prev;
    
    /* xml specific */
    xmlNodePtr				hereNode;

    unsigned char			binBuf[XMLSEC_TRANSFORM_BUFFER_SIZE];
    size_t				binBufSize;
    size_t				processed;
    
    void*				reserved0;
    void*				reserved1;
    void*				reserved2;
    void*				reserved3;
    
    /* xmlSecDigestTransform specific */
    int					pushModeEnabled;
    unsigned char			*digest;
    size_t				digestSize;
    unsigned char			digestLastByteMask;
    void 				*digestData;
};

/** 
 * Digest specific hi-level methods
 */ 
XMLSEC_EXPORT int 	xmlSecDigestSignNode		(xmlSecTransformPtr transform,
							 xmlNodePtr valueNode,
							 int removeOldContent);
XMLSEC_EXPORT int 	xmlSecDigestVerifyNode		(xmlSecTransformPtr transform,
							 const xmlNodePtr valueNode);
XMLSEC_EXPORT void	xmlSecDigestSetPushMode		(xmlSecTransformPtr transform,
							 int enabled);

/** 
 * Digest specific low-level methods
 */ 
XMLSEC_EXPORT int 	xmlSecDigestUpdate		(xmlSecTransformPtr transform,
							 const unsigned char *buffer,
							 size_t size);
XMLSEC_EXPORT int 	xmlSecDigestSign		(xmlSecTransformPtr transform,
							 unsigned char **buffer,
							 size_t *size);
XMLSEC_EXPORT int 	xmlSecDigestVerify		(xmlSecTransformPtr transform,
							 const unsigned char *buffer,
							 size_t size);


/**
 * Transform methods to be used in the Id structure
 */
XMLSEC_EXPORT int  	xmlSecDigestTransformRead	(xmlSecTransformPtr transform, 
							 unsigned char *buf, 
							 size_t size);
XMLSEC_EXPORT int  	xmlSecDigestTransformWrite	(xmlSecTransformPtr transform, 
                                        		 const unsigned char *buf, 
							 size_t size);
XMLSEC_EXPORT int  	xmlSecDigestTransformFlush	(xmlSecTransformPtr transform);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_DIGESTS_H__ */

