package com.ren.ndk_file_patch;

import java.io.File;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class MainActivity extends Activity {
	private String SD_CARD_PATH;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		SD_CARD_PATH = Environment.getExternalStorageDirectory().getAbsolutePath();
	}
	
	public void diff(View view){
		String path = SD_CARD_PATH +File.separatorChar+ "2.mp4";
		String path_pattern = SD_CARD_PATH +File.separatorChar+ "2_%d.mp4";
		PatchUtils.diff(path, path_pattern, 2);
		System.out.println("拆分完成");
	}
	public void patch(View view){
		String merge_path = SD_CARD_PATH +File.separatorChar+ "2_new.mp4";
		String path_pattern = SD_CARD_PATH +File.separatorChar+ "2_%d.mp4";
		PatchUtils.patch(path_pattern, merge_path, 2);
		System.out.println("合并完成");
	}
}
