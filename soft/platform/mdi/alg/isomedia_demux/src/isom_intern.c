
#include "global.h"
#include "mcip_debug.h"
#ifdef MP4_3GP_SUPPORT

#include "isomedia_dev.h"
#include "list.h"

extern GF_BitStream *bs_glob;//add shenh
extern uint8 *pBufGlob;//temp buf. 512bytes .add shenh

GF_Err gf_isom_parse_movie_boxes(GF_ISOFile *mov)
{
	GF_Box *a;
	u64 totSize;
	GF_Err e = GF_OK;
	totSize = 0;

	/*while we have some data, parse our boxes*/
	while (gf_bs_available(mov->movieFileMap->bs) > 8) {
	
		e = gf_isom_parse_box(&a, mov->movieFileMap->bs);
		if(e==GF_INVALID_BOX)//sheen
		{
			totSize += (uint32)a;//recode the size.sheen
			continue;//skip this box
		}
		if (e >= 0) {
			e = GF_OK;
		} else {
			return e;
		}

		switch (a->type) {
		/*MOOV box*/
		case GF_ISOM_BOX_TYPE_MOOV:
			if (mov->moov) return GF_ISOM_INVALID_FILE;
			mov->moov = (GF_MovieBox *)a;
			/*set our pointer to the movie*/
			mov->moov->mov = mov;
			e = gf_list_add2(mov->TopBoxes, a);//change shenh
			if (e) return e;
			totSize += a->size;
			break;

		/*META box*/
//		case GF_ISOM_BOX_TYPE_META:
//			if (mov->meta) return GF_ISOM_INVALID_FILE;
//			mov->meta = (GF_MetaBox *)a;
//			e = gf_list_add(mov->TopBoxes, a);
//			if (e) return e;
//			totSize += a->size;
//			break;

		/*we only keep the MDAT in READ for dump purposes*/
		case GF_ISOM_BOX_TYPE_MDAT:
			totSize += a->size;
			mov->mdat = (GF_MediaDataBox *) a;;
			e = gf_list_add2(mov->TopBoxes, mov->mdat);//change shenh
			if (e) return e;
			break;
		case GF_ISOM_BOX_TYPE_FTYP:
			/*ONE AND ONLY ONE FTYP*/
			if (mov->brand) {
				return GF_ISOM_INVALID_FILE;
			}
			mov->brand = (GF_FileTypeBox *)a;
			totSize += a->size;
			e = gf_list_add2(mov->TopBoxes, a);//change shenh
			break;

		case GF_ISOM_BOX_TYPE_PDIN:
			/*ONE AND ONLY ONE PDIN*/
			if (mov->pdin) {
				return GF_ISOM_INVALID_FILE;
			}
			mov->pdin = (GF_ProgressiveDownloadBox *) a;
			totSize += a->size;
			e = gf_list_add2(mov->TopBoxes, a);//change shenh
			break;
		default:
			totSize += a->size;
			e = gf_list_add2(mov->TopBoxes, a);//change shenh
			break;
		}
	}
	/*we need at least moov or meta*/
	if (!mov->moov ) return GF_ISOM_INVALID_FILE;
	/*we MUST have movie header*/
	if (mov->moov && !mov->moov->mvhd) return GF_ISOM_INVALID_FILE;
	return GF_OK;
}

GF_ISOFile *gf_isom_new_movie()
{
	//diag_printf("mmc_MemMalloc .... gf_isom_new_movie \n");
	GF_ISOFile *mov = (GF_ISOFile*)mmc_MemMalloc(sizeof(GF_ISOFile));
	if (mov == NULL) {
		return NULL;
	}
	memset(mov, 0, sizeof(GF_ISOFile));

	/*init the boxes*/
	mov->TopBoxes = gf_list_new();
	if (!mov->TopBoxes) {
		//free(mov);change shenh
		return NULL;
	}

	//add shenh
	mov->TopBoxes->slots= (void **)mmc_MemMalloc(MAX_TOPBOX_SLOT_NUM*sizeof(void*));
	if (!mov->TopBoxes->slots) {
	//free(mov);change shenh
	return NULL;
	}
	memset(mov->TopBoxes->slots, 0, MAX_TOPBOX_SLOT_NUM*sizeof(void*));
	//

	return mov;
}

int gf_isom_open_file(HANDLE file_handle, uint8 file_mode, uint32 data_Len, GF_ISOFile **movie)
{
	GF_Err e;  
	
	bs_glob=NULL;
	bs_glob=(GF_BitStream*)mmc_MemMalloc(sizeof(GF_BitStream));
	if(!bs_glob)return -1;
	memset(bs_glob, 0, sizeof(GF_BitStream));
	pBufGlob=(unsigned char*)mmc_MemMalloc(BUFGLOB_SIZE);
	if(!pBufGlob)return -1;
	memset(pBufGlob, 0, BUFGLOB_SIZE);
	GF_ISOFile *mov = gf_isom_new_movie();
	if(!mov)return -1;
	
	*movie=mov;
	
	e = gf_isom_datamap_new( file_handle, file_mode, data_Len, &mov->movieFileMap);
	if (e) {			
		return -1;
	}
	//mov->fileName = strdup(fileName);	 change shenh
	/*
	mov->fileName=mmc_MemMalloc(strlen(fileName));
	if(!mov->fileName)return -1;
	memset(mov->fileName, 0, strlen(fileName));
	strcpy(mov->fileName, fileName);
	*/

	e=gf_isom_parse_movie_boxes(mov);

	if (e) {			
		return -1;
	}

    return  0;
}

int gf_isom_close_file(GF_ISOFile * movie)//add shenh
{
	GF_FileDataMap *tmp; 
	if(!movie)
	{
		diag_printf("gf_isom_close_file movie NULL\n");
		return -1;
	}
	if(!movie->movieFileMap)
	{
		diag_printf("gf_isom_close_file movieFileMap NULL\n");
		return -1;
	}
	
	tmp = (GF_FileDataMap *)movie->movieFileMap;

	/*
	tmp->stream==0
	if(tmp->stream)
	{
		diag_printf("gf_isom_close_file stream NULL\n");
		return -1;
	}*/

	//diag_printf("**********CLOSE FILE IN VIDEO********tmp->stream:%d",tmp->stream);
//	FS_Close((INT32)tmp->stream);

    	return  0;
}

GF_TrackBox *gf_isom_get_track(GF_MovieBox *moov, uint32 trackNumber)
{
	GF_TrackBox *trak;
	if (!moov || !trackNumber || (trackNumber > gf_list_count(moov->trackList))) return NULL;
	trak = (GF_TrackBox*)gf_list_get(moov->trackList, trackNumber - 1);
	return trak;

}

GF_TrackBox *gf_isom_get_track_from_file(GF_ISOFile *movie, uint32 trackNumber)
{
	GF_TrackBox *trak;
	if (!movie) return NULL;
	trak = gf_isom_get_track(movie->moov, trackNumber);
	if (!trak) movie->LastError = GF_BAD_PARAM;
	return trak;
}

#endif

