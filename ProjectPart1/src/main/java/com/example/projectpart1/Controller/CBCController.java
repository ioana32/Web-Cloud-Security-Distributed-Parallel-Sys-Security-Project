package com.example.projectpart1.Controller;

import com.example.projectpart1.Model.CBCModel;
import com.example.projectpart1.Service.CBCService;
import com.example.projectpart1.util.Base64Util;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.web.bind.annotation.*;

import javax.jms.Topic;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

import org.springframework.web.multipart.MultipartFile;

import java.io.File;
import java.io.IOException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@CrossOrigin(origins = "http://localhost:3000")
@RestController
@RequestMapping("/cbc")
public class CBCController {

    private static final Logger logger = LoggerFactory.getLogger(CBCController.class);

    private final CBCService encryptionService;

    private final JmsTemplate jmsTemplate;
    private final Topic jmsTopic;
    @Autowired
    public CBCController(CBCService encryptionService, JmsTemplate jmsTemplate, Topic jmsTopic) {
        this.encryptionService = encryptionService;
        this.jmsTemplate = jmsTemplate;
        this.jmsTopic = jmsTopic;
    }

    @PostMapping("/encrypt")
    public Map<String, String> encrypt(@RequestBody CBCModel request) throws Exception {
        byte[] imageData = Files.readAllBytes(Paths.get(request.getFilePath()));
        byte[] key = Base64Util.decode(request.getKey());
        String encryptedData = encryptionService.encrypt(imageData, key, request.getIv());

        Map<String, String> response = new HashMap<>();
        response.put("encryptedData", encryptedData);
        return response;
    }

    @PostMapping("/encrypt2")
    public Map<String, String> encrypt2(@RequestBody CBCModel request) throws Exception {
        logger.info("Received request to encrypt image and send via JMS");

        byte[] imageData = Files.readAllBytes(Paths.get(request.getFilePath()));
        byte[] key = Base64Util.decode(request.getKey());
        byte[] iv = request.getIv();

        logger.info("Encrypting image data");
        String encryptedData = encryptionService.encrypt(imageData, key, iv);
        logger.info("Image data encrypted successfully");

        logger.info("Sending encrypted data to JMS Topic");
        jmsTemplate.convertAndSend(jmsTopic, encryptedData.getBytes());
        logger.info("Encrypted data sent to JMS Topic successfully");

        Map<String, String> response = new HashMap<>();
        response.put("encryptedData", encryptedData);
        return response;
    }


    @PostMapping("/decrypt")
    public Map<String, String> decrypt(@RequestBody CBCModel request) throws Exception {
        byte[] encryptedImageData = Files.readAllBytes(Paths.get(request.getFilePath()));
        byte[] key = Base64Util.decode(request.getKey());
        String decryptedData = encryptionService.decrypt(encryptedImageData, key, request.getIv());

        Map<String, String> response = new HashMap<>();
        response.put("decryptedData", decryptedData);
        return response;
    }

    @PostMapping("/decrypt2")
    public Map<String, String> decrypt2(@RequestBody CBCModel request) throws Exception {
        logger.info("Received request to decrypt image and send via JMS");

        byte[] encryptedImageData = Files.readAllBytes(Paths.get(request.getFilePath()));
        byte[] key = Base64Util.decode(request.getKey());
        String decryptedData = encryptionService.decrypt(encryptedImageData, key, request.getIv());

        logger.info("Sending decrypted data to JMS Topic");
        jmsTemplate.convertAndSend(jmsTopic, decryptedData.getBytes());
        logger.info("Decrypted data sent to JMS Topic successfully");

        Map<String, String> response = new HashMap<>();
        response.put("decryptedData", decryptedData);
        return response;
    }

    @PostMapping("/upload")
    public Map<String, String> handleFileUpload(@RequestParam("file") MultipartFile file) throws IOException {
        String uploadDir = "uploaded-images/";
        File directory = new File(uploadDir);
        if (!directory.exists()) {
            directory.mkdirs();
        }

        String filePath = uploadDir + file.getOriginalFilename();
        Path path = Paths.get(filePath);
        Files.write(path, file.getBytes());

        Map<String, String> response = new HashMap<>();
        response.put("filePath", filePath);
        return response;
    }
}
