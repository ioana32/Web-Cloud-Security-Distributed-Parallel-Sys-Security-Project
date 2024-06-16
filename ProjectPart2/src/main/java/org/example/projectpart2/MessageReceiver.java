package org.example.projectpart2;

import org.springframework.jms.annotation.JmsListener;
import org.springframework.stereotype.Component;

@Component
public class MessageReceiver {

    @JmsListener(destination = "Part1", subscription = "Part1Subscription", containerFactory = "jmsListenerContainerFactory")
    public void receiveMessage(String encryptedMessage) {
        // Aici poți procesa mesajul primit
        System.out.println("Received encrypted message from topic: " + encryptedMessage);
        // Poți adăuga logica pentru a decoda mesajul și a-l procesa în continuare
    }
}

