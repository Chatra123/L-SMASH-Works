/* This file is available under an ISC license. */
/*
 *  #include are brought from lwlibav_source.c
 */
#include <libavformat/avformat.h>       /* Codec specific info importer */
#include <libavcodec/avcodec.h>         /* Decoder */
#include <libswscale/swscale.h>         /* Colorspace converter */
#include <libavutil/imgutils.h>

/* Dummy definitions.
 * Audio resampler/buffer is NOT used at all in this filter. */
typedef void AVAudioResampleContext;
typedef void audio_samples_t;

#include "../common/audio_output.h"

#include "lsmashsource.h"
#include "video_output.h"

#include "../common/progress.h"
#include "../common/lwlibav_dec.h"
#include "../common/lwlibav_video.h"
#include "../common/lwlibav_audio.h"
#include "../common/lwindex.h"



/*==================================
 * CommandLine
 *==================================*/
void ParseCommandLine(int argc, char** argv,
                         crlwi_setting *setting)
{
    char src_path[_MAX_PATH] = { 0 };
    char file_path[_MAX_PATH] = { 0 };
    char lwi_path[_MAX_PATH] = { 0 };
    char file_path_inner_lwi[_MAX_PATH] = { 0 };
    bool mode_pipe_input = false;
    bool is_filename_inner_lwi = false;
    bool create_footer = false;
    double read_limit_MiBsec = 0.0;

    /* parse */
    for ( size_t i = 1; i < argc; i++ )
    {
        /* Is argv[1] filepath ? */
        if ( i == 1 )
        {
            FILE *fp = fopen( argv[1], "r" );
            if ( fp )
            {
                mode_pipe_input = false;
                sprintf( file_path, "%s", argv[1] );
                fclose( fp );
            }
        }

        if ( _stricmp( argv[i], "-file" ) == 0 )
        {
            mode_pipe_input = false;
            if ( i + 1 < argc )
                sprintf( file_path, "%s", argv[i + 1] );
        }
        else if ( _stricmp(argv[i], "-pipe") == 0 )
        {
            mode_pipe_input = true;
            if ( i + 1 < argc )
                sprintf( file_path, "%s", argv[i + 1] );
        }
        else if ( _stricmp( argv[i], "-lwi" ) == 0 )
        {
            if ( i + 1 < argc )
                sprintf( lwi_path, "%s", argv[i + 1] );
        }
        else if ( _stricmp( argv[i], "-footer" ) == 0 )
        {
            create_footer = true;
        }
        else if ( _stricmp( argv[i], "-ref_filename" ) == 0 )
        {
            is_filename_inner_lwi = true;
        }
        else if ( _stricmp( argv[i], "-limit" ) == 0 )
        {
           if ( i + 1 < argc )
               if ( ( _snscanf( argv[i + 1], 6, "%lf", &read_limit_MiBsec ) ) <= 0 )
                  read_limit_MiBsec = 0;
        }
    }

    /* post process */
    if( mode_pipe_input )
        sprintf( src_path, "pipe:0" );
    else
        sprintf( src_path, "%s", file_path );
    if ( !lwi_path[0] )
        sprintf( lwi_path, "%s.lwi", file_path );
    char ext[_MAX_EXT] = { 0 };
    _splitpath( lwi_path, NULL, NULL, NULL, ext );
    if ( _stricmp(ext, ".lwi") != 0 )
    {
        char tmp[_MAX_PATH] = { 0 };
        sprintf( tmp, "%s.lwi", lwi_path );
        sprintf( lwi_path, "%s", tmp );
    }
    if ( is_filename_inner_lwi )
    {
        /* fullpath  -->  filename */
        char name[_MAX_PATH] = { 0 }, ext[_MAX_EXT] = { 0 };
        _splitpath( file_path, NULL, NULL, name, ext );
        sprintf( file_path_inner_lwi, "%s%s", name, ext );
    }
    else
        sprintf( file_path_inner_lwi, "%s", file_path );

    /* result */
    sprintf( setting->src_path, "%s", src_path );
    sprintf( setting->file_path, "%s", file_path );
    sprintf( setting->lwi_path, "%s", lwi_path );
    sprintf( setting->file_path_inner_lwi, "%s", file_path_inner_lwi );
    setting->mode_pipe_input = mode_pipe_input;
    setting->create_footer = create_footer;
    setting->read_limit_MiBsec = read_limit_MiBsec;
}


/*==================================
 * version
 *==================================*/
void show_version()
{
    /* lwindex.c  line 2070 */
    uint8_t lwindex_version[4] =
    {
        (LWINDEX_VERSION >> 24) & 0xff,
        (LWINDEX_VERSION >> 16) & 0xff,
        (LWINDEX_VERSION >>  8) & 0xff,
         LWINDEX_VERSION        & 0xff
    };
    fprintf( stderr, "lwi index version\n");
    fprintf( stderr, "    <LSMASHWorksIndexVersion=%" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 ">\n",
             lwindex_version[0], lwindex_version[1], lwindex_version[2], lwindex_version[3] );
    fprintf( stderr, "    <LibavReaderIndexFile=%d>\n", LWINDEX_INDEX_FILE_VERSION );
    return;
}


/*==================================
 * main
 *==================================*/
#include <unistd.h>  /* sleep */
extern void VS_CC vs_lwlibavsource_create_CrLwi( const VSMap *in, VSMap *out, void *user_data, VSCore *core, const VSAPI *vsapi,
                                                       crlwi_setting *clshp);
int main( int argc, char** argv )
{
    /* test args */
//    char *test_argv[16] = { 0 };
//    test_argv[0] = "app_path";
//    test_argv[1] = "E:/TS_Samp/t2s.ts";
//    argv = test_argv;
//    argc = 2;

    if( argc == 1 )
    {
        show_version();
        sleep(4);
        return 0;
    }
    crlwi_setting setting = { 0 };
    ParseCommandLine( argc, argv, &setting );
    if ( setting.mode_pipe_input == false )
    {
        FILE *fp = fopen( setting.file_path, "r");
        if (!fp )
            return 1;
        fclose( fp );
    }
    vs_lwlibavsource_create_CrLwi( NULL, NULL, NULL, NULL, NULL, &setting );
    return 0;
}

/*
 * lwlibav_source.c
 *   line 240
 *          void VS_CC vs_lwlibavsource_create_CrLwi()
 *
 * common/lwindex.c
 *   line 2000
 *          static void create_index_CrLwi()
 *   line 3470
 *         int lwlibav_construct_index_CrLwi
 */
/*
    int log_lv = av_log_get_level();   //default: AV_LOG_INFO
    av_log_set_level( AV_LOG_INFO );

  [log.h]
    #define     AV_LOG_QUIET   -8
        Print no output. More...
    #define     AV_LOG_PANIC   0
        Something went really wrong and we will crash now. More...
    #define     AV_LOG_FATAL   8
        Something went wrong and recovery is not possible. More...
    #define     AV_LOG_ERROR   16
        Something went wrong and cannot losslessly be recovered. More...
    #define     AV_LOG_WARNING 24
        Something somehow does not look correct. More...
    #define     AV_LOG_INFO    32
        Standard information. More...
*/






