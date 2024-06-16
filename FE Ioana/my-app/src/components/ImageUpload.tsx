import React, { useState, ChangeEvent } from 'react';
import cbcEncrypt from './cbc-encrypt';
import axios from 'axios';

const ImageUpload: React.FC = () => {
    const [selectedImage, setSelectedImage] = useState<File | null>(null);
    const [encryptedPreview, setEncryptedPreview] = useState<string | null>(null);
    const [preview, setPreview] = useState<string | null>(null);
    const [key, setKey] = useState<string>('');

    const handleKeyChange = (event: ChangeEvent<HTMLInputElement>) => {
        setKey(event.target.value);
    };

    const handleImageChange = (event: ChangeEvent<HTMLInputElement>) => {
        const file = event.target.files?.[0];
        if (file) {
            setSelectedImage(file);
            const reader = new FileReader();
            reader.onloadend = () => {
                setPreview(reader.result as string);
            };
            reader.readAsDataURL(file);
        }
    };

    const handleEncrypt = async (mode: 'CBC' | 'ECB') => {
        if (selectedImage) {
            const formData = new FormData();
            formData.append('file', selectedImage);

            try {
                const uploadResponse = await axios.post('http://localhost:8080/cbc/upload', formData, {
                    headers: {
                        'Content-Type': 'multipart/form-data'
                    }
                });

                const filePath = uploadResponse.data.filePath;
                const jsonResponse = {
                    filePath: filePath,
                    key: 'abcdefghijklmnbopjhfdd',
                };
                let response;
                if (mode === 'CBC') {
                    response = await axios.post('http://localhost:8080/cbc/encrypt', jsonResponse);
                } else {
                    response = await axios.post('http://localhost:8080/ecb/encrypt', jsonResponse);
                }
                console.log('Encrypted Data:', response.data.encryptedData);

                const encryptedData = response.data.encryptedData;
                const byteCharacters = atob(encryptedData);
                const byteNumbers = new Array(byteCharacters.length);
                for (let i = 0; i < byteCharacters.length; i++) {
                    byteNumbers[i] = byteCharacters.charCodeAt(i);
                }
                const byteArray = new Uint8Array(byteNumbers);

                // Crează un obiect Blob cu datele decodate
                const blob = new Blob([byteArray], { type: 'image/bmp' });

                // Generează un URL blob
                const url = URL.createObjectURL(blob);

                // Utilizează URL-ul generat pentru a afișa imaginea într-un element <img>
                setEncryptedPreview(url);
            } catch (error) {
                console.error('Error sending data:', error);
            }
        }
    };

    const handleDecrypt = async (mode: 'CBC' | 'ECB') => {
        if (selectedImage) {
            const formData = new FormData();
            formData.append('file', selectedImage);

            try {
                const uploadResponse = await axios.post('http://localhost:8080/ecb/upload', formData, {
                    headers: {
                        'Content-Type': 'multipart/form-data'
                    }
                });

                const filePath = uploadResponse.data.filePath;
                const jsonResponse = {
                    filePath: filePath,
                    key: 'abcdefghijklmnbopjhfdd',
                };

                let response;
                if (mode === 'CBC') {
                    response = await axios.post('http://localhost:8080/cbc/decrypt', jsonResponse);
                } else {
                    response = await axios.post('http://localhost:8080/ecb/decrypt', jsonResponse);
                }
                console.log('Decrypted Data:', response.data.decryptedData);

                const decryptedData = response.data.encryptedData;
                const byteCharacters = atob(decryptedData);
                const byteNumbers = new Array(byteCharacters.length);
                for (let i = 0; i < byteCharacters.length; i++) {
                    byteNumbers[i] = byteCharacters.charCodeAt(i);
                }
                const byteArray = new Uint8Array(byteNumbers);

                // Crează un obiect Blob cu datele decodate
                const blob = new Blob([byteArray], { type: 'image/bmp' });

                // Generează un URL blob
                const url = URL.createObjectURL(blob);

                // Utilizează URL-ul generat pentru a afișa imaginea într-un element <img>
                setEncryptedPreview(url);
            } catch (error) {
                console.error('Error sending data:', error);
            }
        }
    };

    return (
        <div>
            <input
                type="text"
                placeholder="Enter key"
                value={key}
                onChange={handleKeyChange}
                style={{ marginBottom: '20px' }}
            />
            <br />
            <input type="file" accept="image/*" onChange={handleImageChange} />
            {preview && (
                <div style={{ marginTop: '20px' }}>
                    <img src={preview} alt="Preview" style={{ width: '300px', height: 'auto' }} />
                </div>
            )}
            <div style={{ marginTop: '20px' }}>
                <button style={{ margin: '0 10px 10px 0' }} onClick={() => handleEncrypt('CBC')}>CBC Encrypt</button>
                <button style={{ margin: '0 10px 10px 0' }} onClick={() => handleDecrypt('CBC')}>CBC Decrypt</button>
                <button style={{ margin: '0 10px 10px 0' }} onClick={() => handleEncrypt('ECB')}>ECB Encrypt</button>
                <button style={{ margin: '0 10px 10px 0' }} onClick={() => handleDecrypt('ECB')}>ECB Decrypt</button>
            </div>
            {encryptedPreview &&
                <div style={{ marginTop: '20px' }}>
                    <img src={encryptedPreview} alt="Encrypted/Decrypted Preview" style={{ width: '300px', height: 'auto' }} />
                </div>
            }
        </div>
    );
};

export default ImageUpload;
