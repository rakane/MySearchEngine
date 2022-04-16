const axios = require('axios');
const cheerio = require('cheerio');
const fs = require('fs');

// Tags we don't want to scrape
const IGNORE_TAGS = ['script', 'style', 'meta', 'img'];

// Max number of pages to scrape
const PAGE_LIMIT = 500;

// Keep track of pages we processed
let processedPages = [];

// Stored data from scraped pages
const pages = [];

// Inverted index used to store each word and the sites that contain it
const invertedIndex = new Map();

const isValidUrl = (string) => {
    let url;
    
    try {
      url = new URL(string);
    } catch (_) {
      return false;
    }
  
    return url.protocol === "http:" || url.protocol === "https:";
}

const formatUrl = (baseUrl, linkUrl) => {
    // Already valid url
    if(isValidUrl(linkUrl)) {
        return linkUrl;
    } 
    // Relative url, need to add to base
    else if(linkUrl.charAt(0) === '/') {
        const newUrl = baseUrl + linkUrl;
        if(isValidUrl(newUrl)) {
            return newUrl;
        } 
    } else {
        return null;
    }
}

const isFileLink = (url) => {
    let isFile = false;

    if(url.includes('.svg') || url.includes('.jpg') ||
        url.includes('.png')) {
        isFile = true;
    }

    return isFile;
}

const scrape = async (url) => {
    try {
        // Only want to process if we haven't already
        const { data } = await axios.get(url);
        const $ = cheerio.load(data);

        // Get h1 tag
        const h1 = $('h1').text();

        if(!h1) return;

        // Get all
        const text = $('html *').map((idx, elem) => {
            const tagName = elem.name;

            if(!IGNORE_TAGS.includes(tagName)) {
                const content = $(elem).contents().map((idx2, node) => {
                    if(node.type === 'text') {
                        return $(node).text().trim();
                    }
                }).get();
    
                return content;
            } else {
                return '';
            }
        }).get();

        // Filter empty text from a tag, join all by a space, then re-split to turn all sentences into individual words
        let words = text.join(' ').replace(/[.,\/#!$%\^&\*;:{}=\-_`~()]/g,"").split(' ');

        // Trim any extra spaces
        words = words.map((word) => word.trim()).filter((word) => word !== '');

        // Create map of unique words and there occurances in the page
        const wordMap = new Map();

        // Add words to map, tracking count
        words.forEach((word) => {
            const formattedWord = word.toLowerCase();
            if(!wordMap.has(formattedWord)) {
                wordMap.set(formattedWord, 1);
            } else {
                wordMap.set(formattedWord, wordMap.get(formattedWord) + 1);
            }
        })

        // Add data to pages list
        pages.push({site: url, headline: h1, words: wordMap});

        // Find all links on page, and call to scrape them as well
        const links = $('a');
        
        const linksToScrape = [];

        links.each(async (idx, link) => {
            // Get href
            const linkUrl = $(link).attr('href');

            // If we have not hit our number of pages limit
            if(linkUrl && processedPages.length < PAGE_LIMIT)
            {
                const urlObj = new URL(url);
                const baseUrl = `${urlObj.protocol}//${urlObj.hostname}`;

                // Format url
                const formattedUrl = formatUrl(baseUrl, linkUrl);

                if(formattedUrl) {
                    const shouldScrape = !isFileLink(formattedUrl);

                    // If link is a valid formed url and not already processed
                    if(shouldScrape && !processedPages.includes(formattedUrl)) {
                        processedPages.push(formattedUrl);
                        linksToScrape.push(formattedUrl);
                    }
                }
            }
        });

        await Promise.all(linksToScrape.map((link) => scrape(link)));

    } catch(err) {
        console.log(err);
    }
}

const writeIndexToFile = () => {
    console.log('writing to file...');

    try {
        // Clear file and write header
        fs.writeFileSync('index.csv', 'Word,"sitelink1"-"headline1"-"ocurrances1";"sitelink2"-"headline2"-"ocurrances2"\n');     

        invertedIndex.forEach((sites, word) => {
            let line = `${word},`;
            
            sites.forEach((site, idx) => {
                if(idx > 0) {
                    line += ';';
                }
                line += `\`${site.site}\`-\`${site.headline}\`-\`${site.occurances}\``
            })

            line += '\n';

            fs.writeFileSync('index.csv', line, { flag: 'a+' });     
        });

    } catch(err) {

    }
}

const start = async() => {
    // Page to start from, will recursively search any linked pages 
    // until page limit is reached
    const startUrl = 'https://en.wikipedia.org/wiki/Outline_of_mathematics';
    numPagesProcessed = 1;
    processedPages.push(startUrl);

    await scrape(startUrl);


    console.log('Pages indexed: ', pages.length);

    // Create inverted index
    pages.forEach((page) => {
        const { words, site, headline } = page;

        // Loop all words in each page
        words.forEach((occurances, word) => {

            // If missing from index, create and add page to array of sites
            if(!invertedIndex.has(word)) {
                invertedIndex.set(word, [{ site, headline, occurances}]);
            }
            // Add page to list of pages mapped to word
            else {
                const prev =  invertedIndex.get(word);
                invertedIndex.set(word, [...prev, { site, headline, occurances }]);
            }
        })
    });

    writeIndexToFile();
}

start();




