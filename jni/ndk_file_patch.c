#include "com_ren_ndk_file_patch_PatchUtils.h"
#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include<android/log.h>

#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"renzhenming",FORMAT,__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR),"renzhenming",FORMAT,__VA_ARGS__);

long get_file_size(const char *path){
	FILE *fl = fopen(path,"rb");
	fseek(fl,0,SEEK_END);
	return ftell(fl);
}

JNIEXPORT void JNICALL Java_com_ren_ndk_1file_1patch_PatchUtils_diff
  (JNIEnv *env, jclass jclz, jstring path_jstr, jstring path_pattern_jstr, jint file_num){

	//转换文件路径
	const char *path = (*env)->GetStringUTFChars(env,path_jstr,NULL);
	const char *path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);

	//得到分割之后的所有文件路径

	//申请一段连续的内存空间（一个数组）保存所有分割的文件地址
	char **patches = malloc(sizeof(char*)*file_num);

	int i = 0;
	for(;i<file_num;i++){
		patches[i]=malloc(sizeof(char)*100);
		//元素赋值
		//需要分割的文件：C://jason/liuyan.png
		//子文件：C://jason/liuyan_%d.png(path_pattern的格式)
		sprintf(patches[i],path_pattern,(i+1));
		LOGI("patch path:%s",patches[i]);
	}
	//不断读取path文件，循环写入file_num个文件中
		//	整除
		//	文件大小：90，分成9个文件，每个文件10
		//	不整除
		//	文件大小：110，分成9个文件，
		//	前(9-1)个文件为(110/(9-1))=13
		//	最后一个文件(110%(9-1))=6

	//获取文件大小
	int file_size = get_file_size(path);
	//打开这个文件
	FILE *fpr = fopen(path,"rb");

	//整除
	if(file_size % file_num == 0){
		//单个文件大小
		int part = file_size/file_num;

		int i =0;
		//逐一写入设置好的子文件路径中
		for(;i<file_num;i++){
			//从子文件路径打开一个FILE
			FILE *fpw = fopen(patches[i],"wb");
			int j = 0;
			for(;j<part;j++){
				//边读边写
				fputc(fgetc(fpr),fpw);
			}
			fclose(fpw);
		}
	}else{
		//无法整除
		int part = file_size/(file_num-1);
		int i = 0;
		for(;i<file_num-1;i++){
			FILE *fpw = fopen(patches[i],"wb");
			int j=0;
			for(;j<part;j++){
				fputc(fgetc(fpr),fpw);
			}
			fclose(fpw);
		}

		//最后一个子文件
		FILE *fpw = fopen(patches[file_num-1],"wb");
		i = 0;
		for(;i<file_size%(file_num-1);i++){
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpw);
	}

	//释放
	i = 0;
	for(;i<file_num;i++){
		free(patches[i]);
	}
	free(patches);

	(*env)->ReleaseStringUTFChars(env,path_jstr,path);
	(*env)->ReleaseStringUTFChars(env,path_pattern_jstr,path_pattern);
}

/*
 * Class:     com_ren_ndk_file_patch_PatchUtils
 * Method:    patch
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_ren_ndk_1file_1patch_PatchUtils_patch
  (JNIEnv *env, jclass jclz, jstring path_pattern_jstr, jstring merge_path_jstr, jint file_num){
	//字符串转换
	const char *merge_path = (*env)->GetStringUTFChars(env,merge_path_jstr,NULL);
	const char *path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);
	//得到分割后的子文件路径列表
	char **patches = malloc(sizeof(char*)*file_num);
	int i = 0;
	for(;i<file_num;i++){
		patches[i]=malloc(sizeof(char)*100);
		//元素赋值
		//需要分割的文件：C://jason/liuyan.png
		//子文件：C://jason/liuyan_%d.png
		sprintf(patches[i],path_pattern,i+1);
		LOGI("patch path:%s",patches[i]);
	}
	//打开要merge_path为一个FILE
	FILE *fpw = fopen(merge_path,"wb");
	//把所有分割的文件读取一遍，写入到这个总的文件中
	i = 0;
	for(;i<file_num;i++){
		//得到每个子文件的大小
		int file_size = get_file_size(patches[i]);
		//打开每个子文件路径为一个FILE
		FILE *fpr = fopen(patches[i],"rb");
		int j =0;
		for(;j<file_size;j++){
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpr);
	}
	fclose(fpw);

	//释放内存
	i = 0;
	for(;i<file_num;i++){
		free(patches[i]);
	}
	free(patches);

	(*env)->ReleaseStringUTFChars(env,merge_path_jstr,merge_path);
	(*env)->ReleaseStringUTFChars(env,path_pattern_jstr,path_pattern);
}












