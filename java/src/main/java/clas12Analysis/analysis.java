package clas12Analysis;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import org.jlab.groot.ui.ProgressBar;

/**
 * Hello world!
 *
 */
public class analysis {
  private static Histograms hists = new Histograms();
  private static long startTime;
  private static long endTime;
  private static long total_events = 0;
  private static ProgressBar pbar = new ProgressBar();

  private static List<String> getFileNames(String Directory) {
    List<String> results = new ArrayList<String>();
    File[] files = new File(Directory).listFiles();
    // If this pathname does not denote a directory, then listFiles() returns null.
    for (File file : files) {
      if (file.isFile()) {
        results.add(Directory + file.getName());
      }
    }
    return results;
  }

  public static void main(String[] args) throws InterruptedException {
    pbar.setOut(System.err);
    startTime = System.currentTimeMillis();
    List<String> fileNames = getFileNames("/Users/tylern/Data/hipo/rg-k/");

    for (String fname : fileNames) {
      Thread t = new Thread(new Datahandeler(fname, hists));
      t.start();
      t.join();
    }

    System.out.println("Done!");
    endTime = System.currentTimeMillis();
    System.out.println(total_events / (0.001f * (endTime - startTime)));
    // System.exit(0);
  }
}
