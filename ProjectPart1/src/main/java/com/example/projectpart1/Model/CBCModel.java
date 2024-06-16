package com.example.projectpart1.Model;

public class CBCModel {
    private String filePath;
    private String key;
    private byte[] iv;

    public CBCModel() {
        this.iv = "1234567890abcdef".getBytes();
    }

    public String getFilePath() {
        return filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }

    public String getKey() {
        return key;
    }

    public void setKey(String key) {
        this.key = key;
    }

    public byte[] getIv() {
        return iv;
    }
}
