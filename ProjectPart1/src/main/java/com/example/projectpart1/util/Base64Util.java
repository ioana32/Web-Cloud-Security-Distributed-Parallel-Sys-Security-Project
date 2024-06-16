package com.example.projectpart1.util;
import java.util.Base64;

public class Base64Util {
    public static byte[] decode(String data) {
        return Base64.getDecoder().decode(data);
    }

    public static String encode(byte[] data) {
        return Base64.getEncoder().encodeToString(data);
    }
}