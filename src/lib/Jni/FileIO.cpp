#include "Headers/uk_co_lunalux_libos_FileIO.h"
#include "Handlers.h"
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_openFile
 * Signature: (ILjava/lang/String;)I
 */
jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1openFile(JNIEnv * jvm, jobject, jint fileBits, jstring j_path)
{
    const char * path = jvm->GetStringUTFChars(j_path, 0);
    const size path_size = jvm->GetStringUTFLength(j_path);
    return FileIOHandler::getInstance().j_losOpenFile(fileBits,path,path_size);
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_readFile
 * Signature: (I)Ljava/lang/String;
 */
jstring JNICALL Java_uk_co_lunalux_libos_FileIO_n_1readFile(JNIEnv * jvm, jobject, jint file_id)
{
    return jvm->NewStringUTF(FileIOHandler::getInstance().j_losReadFile(file_id));
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_writeFile
 * Signature: (ILjava/lang/String;)V
 */
void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1writeFile(JNIEnv * jvm, jobject, jint file_id, jstring j_path)
{
    FileIOHandler::getInstance().j_losWriteFile(file_id,jvm->GetStringUTFChars(j_path, 0));
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_closeFile
 * Signature: (I)V
 */
void JNICALL Java_uk_co_lunalux_libos_FileIO_n_1closeFile(JNIEnv *, jobject, jint file_id)
{
    FileIOHandler::getInstance().j_losCloseFile(file_id);
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    n_getCurrentResult
 * Signature: (I)Luk/co/lunalux/libos/Defines/losResult;
 */
jint JNICALL Java_uk_co_lunalux_libos_FileIO_n_1getCurrentResult(JNIEnv *, jobject, jint file_id)
{
    return FileIOHandler::getInstance().j_getCurrentResult(file_id);
}
/*
 * Class:     uk_co_lunalux_libos_FileIO
 * Method:    setAssetPath
 * Signature: (Ljava/lang/String;)V
 */
void JNICALL Java_uk_co_lunalux_libos_FileIO_setAssetPath(JNIEnv * jvm, jobject, jstring j_path)
{
    FileIOHandler::getInstance().j_setAssetPath(jvm->GetStringUTFChars(j_path, 0));
}