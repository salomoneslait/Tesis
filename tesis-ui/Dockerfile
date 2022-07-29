FROM node:12-alpine

WORKDIR /movie-analyst-ui

COPY . . 

RUN yarn install --production

CMD [ "node", "/movie-analyst-ui/server.js" ]