
template <typename T>
void fmt(vector<T>& f){
    int n = f.size();
    for(int i = 0;(1 << i) < n;i++){
        for(int j = 0;j < n;j++){
            if(j & (1 << i)){
                f[j] -= f[j ^ (1 << i)];
            }
        }
    }
}
