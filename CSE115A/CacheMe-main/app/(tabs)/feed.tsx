// React Modules
import React, { useEffect, useState } from 'react'
import { SafeAreaView, StyleSheet, FlatList, ActivityIndicator } from "react-native"

// Components
import PostPreview from '../../components/Post'
import Header from '../../components/Header'

// Firebase
import { db } from '../../firebase';
import { GeoPoint, Timestamp, collection, getDocs, orderBy, query } from 'firebase/firestore';


// Set the default number of posts to load for each pagiation ** TODO
const PAGE_SIZE = 20;

const cache = {
    __id: "",
    __imageId: "",
    __userId: "",
    _createdAt: new Timestamp(0, 0),
    caption: "",
    location: new GeoPoint(0, 0),
    numComments: 0,
    numLikes: 0,
    reported: false,
    likeIDs: [] as string[],
}

type CacheData = {
    __id: string; 
    __imageId: string; 
    __userId: string; 
    _createdAt: Timestamp; 
    caption: string; 
    location: GeoPoint; 
    numComments: number; 
    numLikes: number; 
    reported: boolean;
    likeIDs: string[];
}

const Feed = () => {
    const [postsToRender, setPostsToRender] = useState([cache]);
    const [loading, setLoading] = useState(true);
    
    const fetchPosts = async () => {
        const postsList:CacheData[] = [];
        const cacheRef = collection(db, "cache");
        const q = query(cacheRef, orderBy('_createdAt', 'desc'));
        const querySnapshot = await getDocs(q);

        querySnapshot.forEach((cache) => {
            const {
                __id,
                __imageId,
                __userId,
                _createdAt,
                caption,
                location,
                numComments,
                numLikes,
                reported,
                likeIDs,
            } = cache.data();

            postsList.push({
                __id,
                __imageId,
                __userId,
                _createdAt,
                caption,
                location,
                numComments,
                numLikes,
                reported,
                likeIDs,
            });
        });

        setPostsToRender(postsList);

        if (loading) {
            setLoading(false);
        }

    }

    useEffect(() => {
        fetchPosts();
    })

    return(
        <SafeAreaView style={styles.container}>
            {loading ? (
                <ActivityIndicator size="large" />
            ) : (
                <>
                    <Header />
                    <FlatList data={postsToRender} 
                        keyExtractor={(c) => c.__id} 
                        renderItem={({ item }) => {
                            return (
                                <PostPreview id={item.__id}
                                    captionText={item.caption} 
                                    uid={item.__userId} 
                                    image={item.__imageId}
                                    numComments={item.numComments} 
                                    numLikes={item.numLikes} 
                                    location={item.location} 
                                    timePosted={item._createdAt}
                                    likeIDs={item.likeIDs} />
                            );
                        }} 
                        showsVerticalScrollIndicator={false}/>
                </>
                    
            )}
            
        </SafeAreaView>
    );
};

export default Feed;

const styles = StyleSheet.create({
    container: {
        paddingTop: 25,
        margin: 0,
        marginBottom: 50,
        backgroundColor: "#EEF2FF",
    },
})

