async function cbcEncrypt(filepath: string, key: string) {
  const res = await fetch(`http://localhost:8080/cbc/encrypt`, {
    body: JSON.stringify({ filepath: filepath, key: key }),
    headers: {
      "Content-Type": "application/json",
    },
    method: "POST",
  });
  if (res.ok) {
    return res;
  } else {
    const errorMessage = res;
    return Promise.reject(new Error(errorMessage.toString()));
  }
}

export default cbcEncrypt;
