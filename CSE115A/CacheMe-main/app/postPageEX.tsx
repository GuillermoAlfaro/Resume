import React, {useState, useEffect} from 'react';
import { useSearchParams } from 'expo-router';
import PostPage from '../components/PostPageComponents/PostPage'
import { GeoPoint, Timestamp } from 'firebase/firestore';
import { getCacheData, getDistanceBetween, getImage, getPoster, getTimeDifference } from '../components/PostData';

const cache = {
    __id: "",
    __imageId: "",
    __userId: "",
    _createdAt: new Timestamp(0, 0),
    caption: "",
    location: new GeoPoint(0, 0),
    numComments: 0,
    numLikes: 0,
    reported: false
}

const postPageEx = () => {
    const { postId } = useSearchParams();
    const id = postId as string;
    const cache = getCacheData(id);

    // States
    const [likeCount, setLikeCount] = useState(0);
    const [commentCount, setCommentCount] = useState(0);
    const [poster, setPoster] = useState("");
    const [distBetween, setDistBetween] = useState(0);
    const [postLocation, setPostLocation] = useState<GeoPoint>();
    const [imageURI, setImageURI] = useState("");
    const [timePosted, setTimePosted] = useState("");
    const [captionText, setCaptionText] = useState("");
    const [commentIds, setComments] = useState([]);
    const [uid, setUID] = useState("");

    useEffect ( () => {
        console.log("post id: " + id);
        cache.then((c) => {
            if (c) {
                getPoster(c.__userId).then( username => {
                    setPoster(username);
                });
                getImage(c.__imageId).then( async (uri) => {
                    if (uri) {
                        setImageURI(uri);
                    } else {
                        setImageURI("");
                    }
                });
                setTimePosted(getTimeDifference(c._createdAt));
                getDistanceBetween(c.location).then( dist => {
                    setDistBetween(dist);
                })
                setCommentCount(c.numComments);
                setLikeCount(c.numLikes);
                setCaptionText(c.caption);
                setComments(c.comments);
                setPostLocation(c.location);
                setUID(c.__userId);
            }
        }).catch( () => {
            console.log("Could not fetch cache.")
        })
    }, [postId]) 
    
    return (
        <PostPage id={id} 
            userId={uid} 
            username={poster}
            imageRef={imageURI}
            caption={captionText}
            comments={commentIds}
            distBtwn={distBetween}
            timePosted={timePosted}
            location={postLocation ? postLocation : null}
            nLikes={likeCount}
            nComments={commentCount}/>
    )
}

export default postPageEx