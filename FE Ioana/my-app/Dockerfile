# Use the base image
FROM node:14
# Use the base image
#FROM critoma/linux-u20-dev-security-ism
# Set working directory
WORKDIR /app

# Install Node.js and npm
#RUN apt-get update && apt-get install -y nodejs npm

# Copy package.json and package-lock.json (if present)
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application
COPY . .

# Build the React app
RUN npm run build

# Expose port 3000 (or the port your React app is running on)
EXPOSE 3000

# Command to run the React app
CMD ["npm", "start"]