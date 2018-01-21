package com.ren.ndk_file_patch;

public class PatchUtils {
	
	static{
		System.loadLibrary("ndk_file_patch");
	}

	public native static void diff(String path,String path_pattern,int count);
	
	public native static void patch(String path_pattern,String merge_path,int count);
}
