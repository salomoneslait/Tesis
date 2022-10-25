FROM node:12-alpine

WORKDIR /tesis

COPY . . 

RUN yarn install --production

CMD [ "node", "/tesis/index.js" ]