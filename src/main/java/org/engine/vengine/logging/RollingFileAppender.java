package org.engine.vengine.logging;

import java.io.*;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.nio.channels.Channels;

public class RollingFileAppender implements Appender {
    private final Path logFile;
    private final long maxSize;
    private final int maxFiles;
    private OutputStream output;
    private FileChannel channel;
    private final Object lock = new Object();

    public RollingFileAppender(String filePath, long maxSize, int maxFiles) {
        this.logFile = Paths.get(filePath);
        this.maxSize = maxSize;
        this.maxFiles = maxFiles;
        openFile();
    }

    private void openFile() {
        try {
            Files.createDirectories(logFile.getParent());
            channel = FileChannel.open(
                    logFile,
                    StandardOpenOption.CREATE,
                    StandardOpenOption.WRITE,
                    StandardOpenOption.APPEND
            );
            channel.position(channel.size()); // <-- Важно!
            output = Channels.newOutputStream(channel);
        } catch (IOException e) {
            System.err.println("[Logger] Failed to open log file: " + e.getMessage());
        }
    }



    private void roll() {
        try {
            if (channel == null || !channel.isOpen()) return;
            long size = channel.size();
            if (size < maxSize) return;
            close();
            for (int i = maxFiles - 1; i >= 1; i--) {
                Path src = logFile.resolveSibling(logFile.getFileName() + "." + i);
                Path dst = logFile.resolveSibling(logFile.getFileName() + "." + (i + 1));
                if (Files.exists(src)) {
                    Files.move(src, dst, StandardCopyOption.REPLACE_EXISTING);
                }
            }
            Path rolled = logFile.resolveSibling(logFile.getFileName() + ".1");
            Files.move(logFile, rolled, StandardCopyOption.REPLACE_EXISTING);
            openFile();
        } catch (IOException e) {
            System.err.println("[Logger] Rolling failed: " + e.getMessage());
        }
    }

    @Override
    public void append(LogRecord record) {
        synchronized (lock) {
            if (output == null) {
                openFile();
                if (output == null) return;
            }
            try {
                roll();
                String formatted = format(record);
                output.write(formatted.getBytes(StandardCharsets.UTF_8));
                output.flush();
            } catch (IOException e) {
                System.err.println("RollingFileAppender write failed: " + e);
            }
        }
    }

    private String format(LogRecord record) {
        SimpleDateFormat fmt = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
        return String.format("%s [%s] %s %s - %s%n",
                fmt.format(new Date(String.valueOf(record.timestamp))),
                record.threadName,
                record.level,
                record.category,
                record.message);
    }

    @Override
    public void flush() {
        synchronized (lock) {
            try {
                if (channel != null) channel.force(false);
                if (output != null) output.flush();
            } catch (IOException ignored) {}
        }
    }

    @Override
    public void close() {
        synchronized (lock) {
            try {
                if (output != null) {
                    output.close();
                    output = null;
                }
                if (channel != null) {
                    channel.close();
                    channel = null;
                }
            } catch (IOException ignored) {}
        }
    }
}
