// Ronit Neve
// rxn6292 1001856292

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char s[1503], s1[500], s2[500], s3[500];
int n, sa[1503], rank[1503], lcp[1503];
int suffixCompare(const void *xVoidPt, const void *yVoidPt)
{
    int *xPt = (int*) xVoidPt, *yPt = (int*) yVoidPt;
    return strcmp(&s[*xPt], &s[*yPt]);
}
void computeRank()
{
    int i;
    for(i = 0; i < n; i++)
    {
        rank[sa[i]] = i;
    }
}
void computeLCP()
{
    int h, i, j, k;
    h = 0;
    for(i = 0; i < n; i++)
    {
        k = rank[i];
        if(k == 0)
        {
            lcp[k] = (-1);
        }
        else
        {
            j = sa[k - 1];
            while(i + h < n && j + h < n && s[i + h] == s[j + h])
            {
                h++;
            }
            lcp[k] = h;
        }
        if(h > 0)
        {
            h--;
        }
    }
}

int main()
{
    int i, j, k, p, t, ta, tb, tc, pass, sac, templength, dollarPos, hashPos, LCSpos = -1, LCSlength = 1;
    // Scan the substrings and save them in s1, s2, and s3.
    scanf("%s", s1);
    scanf("%s", s2);
    scanf("%s", s3);
    for(i = 0; s1[i] != '\0'; i++)
    {
        s[i] = s1[i];
    }
    dollarPos = i;
    s[i++] = '$';
    for(j = 0; s2[j] != '\0'; j++)
    {
        s[i + j] = s2[j];
    }
    hashPos = i + j;
    s[i + j] = '#';
    j++;
    for(k = 0; s3[k] != '\0'; k++)
    {
        s[i + j + k] = s3[k];
    }
    s[i + j + k] = ' ';
    n = i + j + k + 1;
    
    printf("n is %d\n", n);
    for(i = 0; i < n; i++)
    {
        sa[i] = i;
    }
    qsort(sa, n, sizeof(int), suffixCompare);
    computeRank();
    computeLCP();
   /*
    if(n < 3000)
    {
        printf("i     sa    suffix                              lcp   t s  rank   lcp[rank]\n");
        for(i = 0; i < n; i++)
        {
            t = 0;
            if(sa[i] > dollarPos)
            {
                t++;
            }
            if(sa[i] > hashPos)
            {
                t++;
            }
            printf("%-5d %-5d %-35.35s %-5d %-1d %c  %-5d  %-5d\n", i, sa[i], &s[sa[i]], lcp[i], t, s[i], rank[i], lcp[rank[i]]);
        }
    }*/
    
    for(i = 1; i < n; i++)
    {
        ta = 0;
        tb = 0;
        tc = 0;
        pass = -1;
        sac = 1;
        templength = 0;
       
        if(sa[i - 1] > dollarPos)
        {
            ta++;
        }
        if(sa[i - 1] > hashPos)
        {
            ta++;
        }
        if(sa[i] > dollarPos)
        {
            tb++;
        }
        if(sa[i] > hashPos)
        {
            tb++;
        }
        if(sa[i + sac] > dollarPos)
        {
            tc++;
        }
        if(sa[i + sac] > hashPos)
        {
            tc++;
        }
        
        while(pass == -1)
        {
            if(ta != tb && ta != tc && tb != tc)
            {
                pass = 1;
                if(lcp[i] <= lcp[i + sac])
                {
                    templength = lcp[i];
                }
                else if(lcp[i] > lcp[i + sac])
                {
                    templength = lcp[i + sac];
                }
            }
            else if(ta != tb && ta != tc && tb == tc)
            {
                sac++;
                tc = 0;
                if(sa[i + sac] > dollarPos)
                {
                    tc++;
                }
                if(sa[i + sac] > hashPos)
                {
                    tc++;
                }
                if(lcp[i] <= lcp[i + sac])
                {
                    templength = lcp[i];
                }
                else if(lcp[i] > lcp[i + sac])
                {
                    templength = lcp[i + sac];
                }
            }
            else
            {
                pass = 0;
            }
        }
        
        if(templength >= LCSlength && pass == 1)
        {
            LCSpos = i;
            LCSlength = templength;
            printf("Length %d, x at %d, y ends at %d, z at %d\n", LCSlength, i - 1, i + sac - 1, i + sac);
            printf("%.*s\n", LCSlength, s + sa[LCSpos]);
        }
    }
}
