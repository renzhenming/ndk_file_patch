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

	//ת���ļ�·��
	const char *path = (*env)->GetStringUTFChars(env,path_jstr,NULL);
	const char *path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);

	//�õ��ָ�֮��������ļ�·��

	//����һ���������ڴ�ռ䣨һ�����飩�������зָ���ļ���ַ
	char **patches = malloc(sizeof(char*)*file_num);

	int i = 0;
	for(;i<file_num;i++){
		patches[i]=malloc(sizeof(char)*100);
		//Ԫ�ظ�ֵ
		//��Ҫ�ָ���ļ���C://jason/liuyan.png
		//���ļ���C://jason/liuyan_%d.png(path_pattern�ĸ�ʽ)
		sprintf(patches[i],path_pattern,(i+1));
		LOGI("patch path:%s",patches[i]);
	}
	//���϶�ȡpath�ļ���ѭ��д��file_num���ļ���
		//	����
		//	�ļ���С��90���ֳ�9���ļ���ÿ���ļ�10
		//	������
		//	�ļ���С��110���ֳ�9���ļ���
		//	ǰ(9-1)���ļ�Ϊ(110/(9-1))=13
		//	���һ���ļ�(110%(9-1))=6

	//��ȡ�ļ���С
	int file_size = get_file_size(path);
	//������ļ�
	FILE *fpr = fopen(path,"rb");

	//����
	if(file_size % file_num == 0){
		//�����ļ���С
		int part = file_size/file_num;

		int i =0;
		//��һд�����úõ����ļ�·����
		for(;i<file_num;i++){
			//�����ļ�·����һ��FILE
			FILE *fpw = fopen(patches[i],"wb");
			int j = 0;
			for(;j<part;j++){
				//�߶���д
				fputc(fgetc(fpr),fpw);
			}
			fclose(fpw);
		}
	}else{
		//�޷�����
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

		//���һ�����ļ�
		FILE *fpw = fopen(patches[file_num-1],"wb");
		i = 0;
		for(;i<file_size%(file_num-1);i++){
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpw);
	}

	//�ͷ�
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
	//�ַ���ת��
	const char *merge_path = (*env)->GetStringUTFChars(env,merge_path_jstr,NULL);
	const char *path_pattern = (*env)->GetStringUTFChars(env,path_pattern_jstr,NULL);
	//�õ��ָ������ļ�·���б�
	char **patches = malloc(sizeof(char*)*file_num);
	int i = 0;
	for(;i<file_num;i++){
		patches[i]=malloc(sizeof(char)*100);
		//Ԫ�ظ�ֵ
		//��Ҫ�ָ���ļ���C://jason/liuyan.png
		//���ļ���C://jason/liuyan_%d.png
		sprintf(patches[i],path_pattern,i+1);
		LOGI("patch path:%s",patches[i]);
	}
	//��Ҫmerge_pathΪһ��FILE
	FILE *fpw = fopen(merge_path,"wb");
	//�����зָ���ļ���ȡһ�飬д�뵽����ܵ��ļ���
	i = 0;
	for(;i<file_num;i++){
		//�õ�ÿ�����ļ��Ĵ�С
		int file_size = get_file_size(patches[i]);
		//��ÿ�����ļ�·��Ϊһ��FILE
		FILE *fpr = fopen(patches[i],"rb");
		int j =0;
		for(;j<file_size;j++){
			fputc(fgetc(fpr),fpw);
		}
		fclose(fpr);
	}
	fclose(fpw);

	//�ͷ��ڴ�
	i = 0;
	for(;i<file_num;i++){
		free(patches[i]);
	}
	free(patches);

	(*env)->ReleaseStringUTFChars(env,merge_path_jstr,merge_path);
	(*env)->ReleaseStringUTFChars(env,path_pattern_jstr,path_pattern);
}












